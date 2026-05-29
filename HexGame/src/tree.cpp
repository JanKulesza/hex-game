#include "tree.h"
#include <algorithm>

bool Tree::haveWon(Game::Color who, QMap<int, Game::Color>& state, const QList<QList<int>>& m_graph)
{
	int size = sqrt(m_graph.size());
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

bool Tree::runSimulation()
{
	auto list = m_legalMoves;
	QMap<int, Game::Color> simState; // Initialize simulation state
	for (auto [id, color] : m_state) 
		simState[id] = color;

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(list.begin(), list.end(), g);

	// Choose colors randomly
	Game::Color nextMove = m_currentPlayer;
	for (int i = 0; i < list.length(); i++)
	{
		simState[list[i]] = nextMove;
		nextMove = nextMove == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	}

	return haveWon(m_currentPlayer, simState, m_graph);
}

int Tree::mcts()
{
	return 0;
}

int Tree::getDepth() const
{
	return m_depth;
}

Game::Color Tree::getCurrentPlayer() const
{
	return m_currentPlayer;
}

QList<QList<int>> Tree::getConnections() const
{
	return m_graph;
}