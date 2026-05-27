#include "board.h"

void Board::resetGame()
{
	m_hexagons.clear();
	m_size = 9;
	m_round = 0;
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
	for (int i = 0; i < N; i++)
		m_hexagons.append(new Hexagon(this));

	QList<QPair<int, int>> potentialNeighs = {
		{-1,0},
		{-1,1},
		{0,-1},
		{0,1},
		{1,-1},
		{1,0}
	};
	m_graph = QList<QList<int>>(N, QList<int>(0));
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
	if (m_hexagons[id]->getColor() != Game::Color::Empty || isPlayer && m_currentPlayer != m_playersColor)
		return;

	m_hexagons[id]->setColor(m_currentPlayer);
	m_currentPlayer = m_currentPlayer == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	m_round++;
	emit m_hexagons[id]->colorChanged();
	emit roundChanged();
	emit currentPlayerChanged();
}

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
