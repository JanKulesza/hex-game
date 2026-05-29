#pragma once

#include "hexagon.h"
#include "types.h"
#include <QList>
#include <QtQml>
#include <QObject>

class Board : public QObject
{
	Q_OBJECT
		Q_PROPERTY(int size READ getSize WRITE setSize NOTIFY sizeChanged)
		Q_PROPERTY(int round READ getRound NOTIFY roundChanged)
		Q_PROPERTY(Game::Color currentPlayer READ getCurrentPlayer WRITE setCurrentPlayer NOTIFY currentPlayerChanged)
		Q_PROPERTY(Game::Color playersColor READ getPlayersColor WRITE setPlayersColor NOTIFY playersColorChanged)
		Q_PROPERTY(Game::Color whoStarts READ getWhoStarts WRITE setWhoStarts NOTIFY whoStartsChanged)
		Q_PROPERTY(QList<Hexagon*> hexagons READ getHexagons NOTIFY hexagonsChanged)

public:
	Board() {}

	Q_INVOKABLE void resetGame();
	Q_INVOKABLE void createBoard();
	Q_INVOKABLE void pick(int id, bool isPlayer);

	void setSize(int s);
	void setCurrentPlayer(Game::Color cp);
	void setPlayersColor(Game::Color pc);
	void setWhoStarts(Game::Color ws);
	void setAiDifficulty(Game::Difficulty d);
	int getSize();
	int getRound();
	Game::Color getCurrentPlayer();
	Game::Color getPlayersColor();
	Game::Color getWhoStarts();
	Game::Difficulty getAiDifficulty();
	QList<Hexagon*> getHexagons();
signals:
	void sizeChanged();
	void roundChanged();
	void currentPlayerChanged();
	void playersColorChanged();
	void aiDifficultyChanged();
	void whoStartsChanged();
	void hexagonsChanged();

private:
	int m_size;
	int m_round;
	Game::Color m_currentPlayer;
	Game::Color m_playersColor;
	Game::Color m_whoStarts;
	Game::Difficulty m_aiDifficulty;
	QList<Hexagon*> m_hexagons; // Hexagons row [i % size], col[i - i % size]
	QList<QList<int>> m_graph; // hexagon connections

	QMap<int, Game::Color> getLocalHexagons();
};