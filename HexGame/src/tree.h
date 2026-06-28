#pragma once

#include "types.h"
#include <QList>
#include <QtQml>
#include <QObject>
#include <QtConcurrent/QtConcurrentRun>

class Tree : public QObject
{
	Q_OBJECT

public:
	Tree(QMap<int, Game::Color> state, Game::Color aiC, QList<QList<int>> connections) // Root tree
		: m_graph(connections), aiColor(aiC) {
		// cP is always AI
		m_root = new Node(*this, state, 0, Players::AI, nullptr, -1);
	}

	~Tree() { delete m_root; }

	void mcts(int iterations);
	static bool haveWon(Game::Color who, QMap<int, Game::Color>& state, const QList<QList<int>>& m_graph);
	QMap<int, int> getMovesEval();

private:
	enum Players {
		AI,
		Human
	};

	struct Node
	{
		Tree& tree;

		QMap<int, Game::Color> m_state; // Current board state
		QList<int> m_legalMoves;
		const int m_depth;
		const Players m_currentPlayer;
		const int chosenHexId;

		double m_win = 0;
		double m_vis = 0;
		double m_uct = INFINITY; // Upper Confidence Bound 1 applied to trees, INFINITY so each child is visited at least once

		Node* parent;
		QList<Node*> children = {};

		bool runSimulation();

		Node(Tree& t, const QMap<int, Game::Color>& state, int d, Players cP, Node* p, int id) :
			tree(t), m_state(state), m_depth(d), m_currentPlayer(cP), parent(p), chosenHexId(id) {
			for (auto i = m_state.cbegin(), end = m_state.cend(); i != end; ++i) // QMap guarantees sorted, set legal moves on initialization
				if (i.value() == Game::Color::Empty)
					m_legalMoves.append(i.key());
		}

		~Node() {
			qDeleteAll(children);
		}
	};

	Game::Color getColorOfPlayer(Players player);
	Node* m_root;
	QList<QList<int>> m_graph;
	const Game::Color aiColor;
};

// Tree Worker
class TreeWorker : public QObject
{
	Q_OBJECT
public:
	TreeWorker(
		QMap<int, Game::Color> state,
		Game::Color aiC,
		const QList<QList<int>> connections,
		Game::Difficulty difficulty,
		QObject* parent = nullptr)
		: QObject(parent), m_graph(connections), m_aiColor(aiC), m_state(state)
	{
		switch (difficulty)
		{
		case Game::Easy:iterations = 1000; break;
		case Game::Medium: iterations = 10000; break;
		case Game::Hard: iterations = 50000; break;
		case Game::Expert: iterations = 100000; break;
		default:
			break;
		}
	}
public slots:
	void doWork() {
		int threadsCount = QThread::idealThreadCount();
		if (threadsCount < 1)
			threadsCount = 4;
		int iterationsPerThread = iterations / threadsCount;

		QList<QFuture<QMap<int, int>>> futures;
		for (int i = 0; i < threadsCount; i++)
		{
			futures.append(QtConcurrent::run([this, iterationsPerThread]() {
				Tree t(m_state, m_aiColor, m_graph);
				t.mcts(iterationsPerThread);
				return t.getMovesEval();
				}));
		}

		QMap<int, int> results;
		for (auto future : futures) {
			QMap<int, int> threadResult = future.result(); 
			for (auto it = threadResult.cbegin(); it != threadResult.cend(); ++it) {
				results[it.key()] += it.value(); 
			}
		}

		int bestMoveId = -1, maxVis = -1;
		for (auto it = results.cbegin(); it != results.cend(); ++it) {
			if (it.value() > maxVis) {
				maxVis = it.value();
				bestMoveId = it.key();
			}
		}

		emit resultReady(bestMoveId, false);
	}

signals:
	void resultReady(const int id, bool isPlayer);

private:
	QList<QList<int>> m_graph;
	const Game::Color m_aiColor;
	QMap<int, Game::Color> m_state;
	int iterations = 0;
};
