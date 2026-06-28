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

bool Tree::haveWon(Game::Color who, QHash<uint8_t, Game::Color>& state, const QList<QList<uint8_t>>& graph)
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

	auto s_legalMoves = this->m_legalMoves;
	auto s_state = this->m_state; // Initialize simulation state

	std::shuffle(s_legalMoves.begin(), s_legalMoves.end(), g);

	// Choose colors randomly
	Game::Color nextMove = tree->getColorOfPlayer(m_currentPlayer);
	for (int i = 0; i < s_legalMoves.length(); i++)
	{
		s_state[s_legalMoves[i]] = nextMove;
		nextMove = nextMove == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	}

	return haveWon(tree->getColorOfPlayer(Tree::Players::AI), s_state, tree->m_graph);

}

uint8_t Tree::mcts(int iterations)
{
	Node* curNode = this->m_root;
	while (iterations > 0)
	{
		// Selection
		uint8_t selectedHexId;
		bool isTerminalNode = false;

		if (curNode->m_legalMoves.size() > 0) { // Leaf selection
			selectedHexId = curNode->m_legalMoves[0];
			curNode->m_legalMoves.pop_front();
		}
		else if (curNode->children.size() > 0) { // Going down the tree
			double maxUCT = -1;
			Node* nextNode = nullptr;
			for (auto it = curNode->children.cbegin(), end = curNode->children.cend(); it != end; ++it) {
				auto c = it.value();
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
		else // If no legal moves and no children, then leaf node is terminal
			isTerminalNode = true;

		// Expansion
		if (!isTerminalNode) {
			Node* expandedNode = new Node(
				curNode->tree,
				curNode->m_state,
				curNode->m_currentPlayer == Tree::Players::AI ? Tree::Players::Human : Tree::Players::AI,
				curNode,
				selectedHexId
			);
			expandedNode->m_state[selectedHexId] = curNode->tree->getColorOfPlayer(curNode->m_currentPlayer);
			curNode->children[selectedHexId] = expandedNode;
			curNode = expandedNode;

		}

		// Simulation
		int aiWonSimulations = curNode->runSimulation() ? 1 : 0;

		// Backpropagation
		while (curNode->parent != nullptr)
		{
			curNode->m_vis++;
			curNode->m_win += aiWonSimulations;
			curNode = curNode->parent;
		}

		curNode->m_vis++;
		curNode->m_win += aiWonSimulations;

		iterations--;
	}

	uint8_t bestMoveId = -1;
	int maxVis = -1;
	for (auto c : m_root->children) {
		if (c->m_vis > maxVis) {
			maxVis = c->m_vis;
			bestMoveId = c->chosenHexId;
		}
	}
	return bestMoveId;
}


// pareller tress doesn't work we need only one source of thruth
void TreeWorker::doWork() {
	Tree t(m_state, m_aiColor, m_graph);
	uint8_t bestMoveId = t.mcts(iterations);

	emit resultReady(bestMoveId, false);
}