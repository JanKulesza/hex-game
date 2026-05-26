#pragma once

#include "hexagon.h"
#include "types.h"
#include <vector>
#include <QtQml>
#include <QObject>

class Board : public QObject
{
	Q_OBJECT
		Q_PROPERTY(int size READ getSize WRITE setSize NOTIFY sizeChanged)
		Q_PROPERTY(Game::Color currentPlayer READ getCurrentPlayer WRITE setCurrentPlayer NOTIFY currentPlayerChanged)
		Q_PROPERTY(Game::Color playersColor READ getPlayersColor WRITE setPlayersColor NOTIFY playersColorChanged)
		Q_PROPERTY(Game::Color whoStarts READ getWhoStarts WRITE setWhoStarts NOTIFY whoStartsChanged)
		Q_PROPERTY(Game::Difficulty aiDifficulty READ getAiDifficulty WRITE setAiDifficulty NOTIFY aiDifficultyChanged)

public:
	Board() {}

	Q_INVOKABLE void resetGame();

	void setSize(int s);
	void setCurrentPlayer(Game::Color cp);
	void setPlayersColor(Game::Color pc);
	void setWhoStarts(Game::Color ws);
	void setAiDifficulty(Game::Difficulty d);
	int getSize();
	Game::Color getCurrentPlayer();
	Game::Color getPlayersColor();
	Game::Color getWhoStarts();
	Game::Difficulty getAiDifficulty();
signals:
	void sizeChanged();
	void currentPlayerChanged();
	void playersColorChanged();
	void aiDifficultyChanged();
	void whoStartsChanged();

private:
	int m_size;
	Game::Color m_currentPlayer;
	Game::Color m_playersColor;
	Game::Color m_whoStarts;
	Game::Difficulty m_aiDifficulty;
	std::vector<std::vector<Hexagon*>> matrix;

};