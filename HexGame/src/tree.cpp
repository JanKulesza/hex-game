#include "tree.h"
#include <algorithm>
#include <random>

//	TO DO: Multithreading
//	TO DO: DSU
//	TO DO: optimize storage allocation eg. int -> uint8_t

static std::random_device rd;
static std::mt19937 g(rd());

Game::Color Tree::getColorOfPlayer(Players player)
{
	return player == Players::AI ? aiColor :
		aiColor == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
}

bool Tree::haveWon(Game::Color who, QMap<int, Game::Color>& state, const QList<QList<int>>& m_graph)
{
	int size = qSqrt(m_graph.size());
	QList<int> stack;
	QSet<int> visited, winnablePos;

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
		int curId = stack.back();
		stack.pop_back();

		if (winnablePos.contains(curId))
			return true;

		if (visited.contains(curId))
			continue;
		visited.insert(curId);

		for (auto vertex : m_graph[curId])
			if (state[vertex] == who && !visited.contains(vertex))
				stack.append(vertex);
	}
	return false;
}

bool Tree::Node::runSimulation() // Check if current player in node have won simulation
{
	auto list = m_legalMoves;
	auto simState = m_state; // Initialize simulation state

	std::shuffle(list.begin(), list.end(), g);

	// Choose colors randomly
	Game::Color nextMove = tree.getColorOfPlayer(m_currentPlayer);
	for (int i = 0; i < list.length(); i++)
	{
		simState[list[i]] = nextMove;
		nextMove = nextMove == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	}

	return haveWon(tree.getColorOfPlayer(m_currentPlayer), simState, tree.m_graph);
}

void Tree::mcts(int iterations)
{

	Node* curNode = this->m_root;
	while (iterations > 0)
	{
		// Selection
		int selectedHexId;
		bool isTerminalNode = false;

		if (curNode->m_legalMoves.size() > 0) {
			std::uniform_int_distribution<int> dist(0, curNode->m_legalMoves.size() - 1);
			int indexToRemove = dist(g);
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
				qDebug("Next node is nullptr!");
				nextNode = curNode->children[0];
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
				curNode->m_depth + 1,
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

QMap<int, int> Tree::getMovesEval()
{
	QMap<int, int> eval;
	for (Node* c : m_root->children) {
		eval[c->chosenHexId] = c->m_vis;
	}
	return eval;
}