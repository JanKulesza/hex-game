#include "tree.h"
#include <random>

//	TO DO: DSU
//	TO DO: optimize storage allocation eg. int -> uint8_t

static std::random_device rd;
static std::mt19937 g(rd());

Game::Color Tree::getColorOfPlayer(Players player)
{
	return player == Players::AI ? aiColor :
		aiColor == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
}

QMap<uint8_t, int> Tree::getMovesEval()
{
	QMap<uint8_t, int> eval;
	for (Node* c : m_root->children) {
		eval[c->chosenHexId] = c->m_vis;
	}
	return eval;
}

bool Tree::haveWon(Game::Color who, QMap<uint8_t, Game::Color>& state, const QList<QList<uint8_t>>& graph)
{
	uint8_t size = qSqrt(graph.size());
	QList<uint8_t> stack;
	QSet<uint8_t> visited, winnablePos;

	// Start vertices
	for (int i = 0;
		i < (who == Game::Color::Red ? size : size * size);
		i = (who == Game::Color::Red ? i + 1 : i + size)) {
		if (state[i] == who)
			stack.append(i);
		if (who == Game::Color::Red) // Set winnable positions
			winnablePos.insert(size * (size - 1) + i);
		else
			winnablePos.insert(i + size - 1);
	}

	// DFS
	while (!stack.isEmpty())
	{
		uint8_t curId = stack.back();
		stack.pop_back();

		if (winnablePos.contains(curId))
			return true;

		if (visited.contains(curId))
			continue;
		visited.insert(curId);

		for (auto vertex : graph[curId])
			if (state[vertex] == who && !visited.contains(vertex))
				stack.append(vertex);
	}
	return false;
}

bool Tree::Node::runSimulation() // Check if current player in node have won simulation
{
	auto s_legalMoves = m_legalMoves;
	auto s_state = m_state; // Initialize simulation state

	std::shuffle(s_legalMoves.begin(), s_legalMoves.end(), g);

	// Choose colors randomly
	Game::Color nextMove = tree.getColorOfPlayer(m_currentPlayer);
	for (int i = 0; i < s_legalMoves.length(); i++)
	{
		s_state[s_legalMoves[i]] = nextMove;
		nextMove = nextMove == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	}

	return haveWon(tree.getColorOfPlayer(m_currentPlayer), s_state, tree.m_graph);
}

void Tree::mcts(int iterations)
{
	Node* curNode = this->m_root;
	while (iterations > 0)
	{
		// Selection
		uint8_t selectedHexId;
		bool isTerminalNode = false;

		if (curNode->m_legalMoves.size() > 0) {
			std::uniform_int_distribution<int> dist(0, curNode->m_legalMoves.size() - 1);
			uint8_t indexToRemove = dist(g);
			selectedHexId = curNode->m_legalMoves[indexToRemove];
			curNode->m_legalMoves.takeAt(indexToRemove);
		}
		else if (curNode->children.size() > 0) {
			double maxUCT = -1;
			Node* nextNode = nullptr;
			for (auto c : curNode->children) {
				if (c->m_vis > 0.0)
					c->m_uct = (c->m_win / c->m_vis) + 1.41 * qSqrt(qLn(curNode->m_vis) / c->m_vis);

				if (c->m_uct > maxUCT) {
					maxUCT = c->m_uct;
					nextNode = c;
				}
			}

			if (nextNode == nullptr) {
				throw "Next node is nullptr!";
			}
			curNode = nextNode;
			continue;
		}
		else
			isTerminalNode = true;

		bool aiWonSimulation;

		// Expansion
		if (!isTerminalNode) {
			Node* expandedNode = new Node(
				curNode->tree,
				curNode->m_state,
				curNode->m_currentPlayer == Tree::Players::AI ? Tree::Players::Human : Tree::Players::AI,
				curNode,
				selectedHexId
			);
			expandedNode->m_state[selectedHexId] = curNode->tree.getColorOfPlayer(curNode->m_currentPlayer);
			curNode->children.append(expandedNode);
			curNode = expandedNode;

			// Simulation
			aiWonSimulation = curNode->runSimulation()
				? curNode->m_currentPlayer == Tree::Players::AI
				: curNode->m_currentPlayer != Tree::Players::AI;
		}
		else
			aiWonSimulation = haveWon(getColorOfPlayer(Players::AI), curNode->m_state, m_graph);

		// Backpropagation
		while (curNode->parent != nullptr)
		{
			curNode->m_vis++;
			if (aiWonSimulation)
				curNode->m_win++;
			curNode = curNode->parent;
		}

		curNode->m_vis++;
		if (aiWonSimulation)
			curNode->m_win++;

		iterations--;
	}
}

void TreeWorker::doWork() {
	int threadsCount = QThread::idealThreadCount();
	if (threadsCount < 1)
		threadsCount = 4;
	QList<QFuture<QMap<uint8_t, int>>> futures;
	for (int i = 0; i < threadsCount; i++)
		futures.append(QtConcurrent::run([this]() {
			Tree t(m_state, m_aiColor, m_graph);
			t.mcts(this->iterations);
			return t.getMovesEval();
			}));

	QMap<uint8_t, int> results;
	for (auto future : futures) {
		auto threadResult = future.result();
		for (auto it = threadResult.cbegin(); it != threadResult.cend(); ++it) {
			results[it.key()] += it.value();
		}
	}

	uint8_t bestMoveId = -1;
	int maxVis = -1;
	for (auto it = results.cbegin(); it != results.cend(); ++it) {
		if (it.value() > maxVis) {
			maxVis = it.value();
			bestMoveId = it.key();
		}
	}

	emit resultReady(bestMoveId, false);
}