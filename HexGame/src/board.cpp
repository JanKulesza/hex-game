#include "board.h"

void Board::resetGame()
{
	m_hexagons.clear();
	m_size = 9;
	m_currentPlayer = m_playersColor = m_whoStarts = Game::Color::Blue;
	m_aiDifficulty = Game::Difficulty::Medium;
	emit sizeChanged();
	emit playersColorChanged();
	emit whoStartsChanged();
	emit aiDifficultyChanged();
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
	if(m_hexagons.isEmpty())
		for (int i = 0; i < m_size*m_size; i++)
			m_hexagons.append(new Hexagon());
	return m_hexagons;
}
