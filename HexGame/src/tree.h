#pragma once

#include "types.h"
#include <algorithm>
#include <QList>
#include <QtQml>
#include <QObject>
#include <QtConcurrent/QtConcurrentRun>

class Tree : public QObject
{
	Q_OBJECT

public:
	// Root tree
	Tree(
		const QHash<uint8_t, Game::Color>& state,
		Game::Color aiColor,
		const QList<QList<uint8_t>>& graph
	)
		: m_graph(graph), aiColor(aiColor)
	{
		m_root = new Node(this, state, Players::AI, nullptr, -1); // cP is always AI
	}
	~Tree() { delete m_root; }

	uint8_t mcts(int iterations);
	static bool haveWon(Game::Color who, QHash<uint8_t, Game::Color>& state, const QList<QList<uint8_t>>& graph);

private:
	enum Players {
		AI,
		Human
	};

	struct Node
	{
		Node(
			Tree* parentTree,
			const QHash<uint8_t, Game::Color> state,
			Players currentPlayer,
			Node* parentNode, 
			uint8_t idOfChosenHex
		)
			: tree(parentTree), m_state(state), m_currentPlayer(currentPlayer), parent(parentNode), chosenHexId(idOfChosenHex)
		{
			for (auto it = m_state.cbegin(), end = m_state.cend(); it != end; ++it) // QMap guarantees sorted, set legal moves on initialization
				if (it.value() == Game::Color::Empty)
					m_legalMoves.append(it.key());
		}

		~Node() {
			qDeleteAll(children);
		}

		Tree* tree;

		QHash<uint8_t, Game::Color> m_state; // Current board state
		QList<uint8_t> m_legalMoves;
		const Players m_currentPlayer;
		const uint8_t chosenHexId;

		double m_win = 0;
		double m_vis = 0;
		double m_uct = INFINITY; // Upper Confidence Bound 1 applied to trees, INFINITY so each child is visited at least once

		Node* parent;
		QHash<uint8_t,Node*> children;

		bool runSimulation();
	};

	const Game::Color aiColor;
	Node* m_root;
	const QList<QList<uint8_t>>& m_graph;

	Game::Color getColorOfPlayer(Players player);
};



// Tree Worker
class TreeWorker : public QObject
{
	Q_OBJECT

public:
	TreeWorker(
		QHash<uint8_t, Game::Color> state,
		const Game::Color aiColor,
		QList<QList<uint8_t>>& graph,
		Game::Difficulty difficulty,
		QObject* parent = nullptr
	)
		: QObject(parent), m_graph(graph), m_aiColor(aiColor), m_state(state)
	{
		switch (difficulty)
		{
		case Game::Difficulty::Easy:   iterations = 10000; break;
		case Game::Difficulty::Medium: iterations = 50000; break;
		case Game::Difficulty::Hard:   iterations = 100000; break;
		case Game::Difficulty::Expert: iterations = 200000; break;
		default:
			break;
		}
	}
public slots:
	void doWork();

signals:
	void resultReady(const uint8_t id, bool isPlayer);

private:
	int iterations = 0;
	const Game::Color m_aiColor;
	const QList<QList<uint8_t>>& m_graph;
	const QHash<uint8_t, Game::Color> m_state;
};
