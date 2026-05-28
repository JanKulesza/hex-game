#include "board.h"
#include <algorithm>
#include <random>

void Board::resetGame()
{
	m_hexagons.clear();
	m_cachedLwh.clear();
	m_size = 9;
	m_round = 1;
	m_currentPlayer = m_playersColor = m_whoStarts = Game::Color::Blue;
	m_aiDifficulty = Game::Difficulty::Medium;
	emit sizeChanged();
	emit roundChanged();
	emit playersColorChanged();
	emit whoStartsChanged();
	emit aiDifficultyChanged();
}

void Board::createBoard()
{
	if (!m_hexagons.isEmpty())
		return;
	const int N = m_size * m_size;

	// Initialize Hexagons
	for (int i = 0; i < N; i++)
		m_hexagons.append(new Hexagon(i, this));

	QList<QPair<int, int>> potentialNeighs = {
		{-1,0},
		{-1,1},
		{0,-1},
		{0,1},
		{1,-1},
		{1,0}
	};
	m_graph = QList<QList<int>>(N, QList<int>(0));
	// Populate m_graph with ids of connected hexagons
	for (int i = 0; i < N; i++)
	{
		int y = i / m_size, x = i % m_size;
		for (auto [diffY, diffX] : potentialNeighs)
			if (y + diffY >= 0 && y + diffY < N && x + diffX >= 0 && x + diffX < N)
				m_graph[i].append((y + diffY) * m_size + x + diffX);
	}
}

Q_INVOKABLE void Board::pick(int id, bool isPlayer)
{
	// Validate move
	if (m_hexagons[id]->getColor() != Game::Color::Empty || isPlayer && m_currentPlayer != m_playersColor)
		return;
	// Make move
	m_hexagons[id]->setColor(m_currentPlayer);
	m_currentPlayer = m_currentPlayer == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	m_round++;
	m_cachedLwh.clear();
	emit m_hexagons[id]->colorChanged();
	emit roundChanged();
	emit currentPlayerChanged();

	// If isPlayer make AI move
	if (isPlayer)
		aiMove();
}

bool Board::haveWon(Game::Color who, QList<lwHexagon>& l_hexagons)
{
	QList<int> stack;
	QSet<int> visited, winnablePos;

	// Start vertices
	for (int i = 0;
		i < (who == Game::Color::Red ? m_size : m_size * m_size);
		i = (who == Game::Color::Red ? i + 1 : i + m_size)) {
		if (l_hexagons[i].color == who)
			stack.append(i);
		if (who == Game::Color::Red) // Set winnable positions
			winnablePos.insert(m_size * (m_size - 1) + i);
		else
			winnablePos.insert(i + m_size - 1);
	}
		
	
	// DFS
	while (!stack.isEmpty())
	{
		int curId = stack.back();
		stack.pop_back();
		if (visited.contains(curId))
			continue;
		if (winnablePos.contains(curId))
			return true;
		
		visited.insert(curId);
		for (auto vertex : m_graph[curId])
			if (l_hexagons[vertex].color == who && !visited.contains(vertex))
				stack.append(vertex);
	}
	return false;
}

void Board::shuffle(QList<lwHexagon>& list)
{
	// Correct order for shuffle Red, Blue, Empty
	std::sort(list.begin(), list.end(), [](lwHexagon a, lwHexagon b) -> bool {
		return a.color < b.color;
		});
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(list.begin() + m_round, list.end(), g);

	// Choose colors randomly
	Game::Color nextMove = m_playersColor;
	for (int i = m_round; i < list.length(); i++)
	{
		list[i].color = nextMove;
		nextMove = nextMove == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	}

	std::sort(list.begin(), list.end(), [](lwHexagon a, lwHexagon b) -> bool {
		return a.id < b.id;
		});
}



QList<lwHexagon> Board::getLocalHexagons() {
	if (!m_cachedLwh.isEmpty())
		return m_cachedLwh;
	QList<lwHexagon> lwh;
	for (Hexagon* h : m_hexagons) 
		lwh.append({ h->id, h->getColor() });
	
	m_cachedLwh = lwh;
	return lwh;
}

int Board::monteCarloEval(const int id)
{
	int won = 0;
	// local m_hexagons
	auto l_hexagons = getLocalHexagons();
	// Choose move to analyze
	l_hexagons[id].color = m_currentPlayer;
	// Analyze move with Monte Carlo
	for (int i = 0; i < 10000; i++)
	{
		shuffle(l_hexagons);
		if (haveWon(m_currentPlayer, l_hexagons))
			won++;
		l_hexagons = getLocalHexagons();
		l_hexagons[id].color = m_currentPlayer;
	}

	return won;
}

void Board::aiMove()
{
	const int N = m_size * m_size;
	QList<int> eval(N, 0);
	int maxEval = -1, evaluatedPos = -1;
	for (size_t i = 0; i < N; i++)
	{
		if (m_hexagons[i]->getColor() != Game::Color::Empty)
			continue;
		// For each possible move evaluate it
		eval[i] = monteCarloEval(i);
		if (eval[i] > maxEval) {
			maxEval = eval[i];
			evaluatedPos = i;
		}
	}
	// Make move
	pick(evaluatedPos, false);
}

// Getters and setters

void Board::setSize(int s)
{
	if (s == m_size) return;
	m_size = s;
	emit sizeChanged();
}

void Board::setCurrentPlayer(Game::Color cp)
{
	if (cp == m_currentPlayer) return;
	m_currentPlayer = cp;
	emit currentPlayerChanged();
}

void Board::setPlayersColor(Game::Color pc)
{
	if (pc == m_playersColor) return;
	m_playersColor = pc;
	emit playersColorChanged();
}

void Board::setWhoStarts(Game::Color ws)
{
	if (ws == m_whoStarts) return;
	m_currentPlayer = m_whoStarts = ws;
	emit whoStartsChanged();
	emit currentPlayerChanged();
}

void Board::setAiDifficulty(Game::Difficulty d)
{
	if (d == m_aiDifficulty) return;
	m_aiDifficulty = d;
	emit aiDifficultyChanged();
}

int Board::getSize()
{
	return m_size;
}

int Board::getRound()
{
	return m_round;
}

Game::Color Board::getCurrentPlayer()
{
	return m_currentPlayer;
}

Game::Color Board::getPlayersColor()
{
	return m_playersColor;
}

Game::Color Board::getWhoStarts()
{
	return m_whoStarts;
}

Game::Difficulty Board::getAiDifficulty()
{
	return m_aiDifficulty;
}

QList<Hexagon*> Board::getHexagons()
{
	return m_hexagons;
}