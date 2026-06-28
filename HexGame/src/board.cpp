#include "board.h"
#include "tree.h"
#include <algorithm>
#include <random>

void Board::resetGame()
{
	m_hexagons.clear();
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
	const uint8_t N = m_size * m_size;

	// Initialize Hexagons
	for (uint8_t i = 0; i < N; i++)
		m_hexagons.append(new Hexagon(i, this));

	QList<QPair<uint8_t, uint8_t>> potentialNeighs = {
		{-1,0},
		{-1,1},
		{0,-1},
		{0,1},
		{1,-1},
		{1,0}
	};
	m_graph = QList<QList<uint8_t>>(N, QList<uint8_t>(0));
	// Populate m_graph with ids of connected hexagons
	for (uint8_t i = 0; i < N; i++)
	{
		uint8_t y = i / m_size, x = i % m_size;
		for (auto [diffY, diffX] : potentialNeighs)
			if (y + diffY >= 0 && y + diffY < N && x + diffX >= 0 && x + diffX < N)
				m_graph[i].append((y + diffY) * m_size + x + diffX);
	}

	if (m_currentPlayer != m_playersColor)
		startThread();
}

QMap<uint8_t,Game::Color> Board::getLocalHexagons() {
	QMap<uint8_t, Game::Color> lwh;
	for (Hexagon* h : m_hexagons)
		lwh[h->id] = h->getColor();

	return lwh;
}

void Board::startThread()
{
	m_aiThinking = true;
	QThread* thread = new QThread(this);
	TreeWorker* worker = new TreeWorker(getLocalHexagons(), m_currentPlayer, m_graph, m_aiDifficulty);

	worker->moveToThread(thread);

	connect(thread, &QThread::started, worker, &TreeWorker::doWork);

	connect(worker, &TreeWorker::resultReady, this, &Board::pick, Qt::QueuedConnection);

	connect(worker, &TreeWorker::resultReady, thread, &QThread::quit);
	connect(thread, &QThread::finished, worker, &QObject::deleteLater);
	connect(thread, &QThread::finished, thread, &QObject::deleteLater);

	thread->start();
}

Q_INVOKABLE void Board::pick(const uint8_t id, bool isPlayer)
{
	if (!isPlayer)
		m_aiThinking = false;
	// Validate move
	if (m_hexagons[id]->getColor() != Game::Color::Empty || isPlayer && m_currentPlayer != m_playersColor || m_aiThinking)
		return;
	// Make move
	m_round++;
	m_hexagons[id]->setColor(m_currentPlayer);
	m_currentPlayer = m_currentPlayer == Game::Color::Blue ? Game::Color::Red : Game::Color::Blue;
	emit roundChanged();
	emit currentPlayerChanged();

	// If isPlayer make AI move
	if (isPlayer)
		startThread();
}

// Getters and setters

void Board::setSize(uint8_t s)
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

uint8_t Board::getSize()
{
	return m_size;
}

uint8_t Board::getRound()
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