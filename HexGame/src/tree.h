#pragma once

#include "hexagon.h"
#include "types.h"
#include <QList>
#include <QtQml>
#include <QObject>

class Tree : public QObject
{
	Q_OBJECT

public:
	Tree(const QList<QList<int>> connections, Game::Color aiC) // Empty tree
		: m_graph(connections), m_node(nullptr), aiColor(aiC) {
	}
	Tree(QMap<int, Game::Color> state, Game::Color aiC, const QList<QList<int>> connections) // Root tree
		: m_graph(connections), aiColor(aiC) {
		// cP is always AI
		m_node = new Node(*this, state, 0, Players::AI, nullptr, -1);
	}

	~Tree() { delete m_node; }

	QList<QList<int>> getConnections() const;

	int mcts(int iterations);
	static bool haveWon(Game::Color who, QMap<int, Game::Color>& state, const QList<QList<int>>& m_graph);

signals:

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
			for (auto i = m_state.cbegin(), end = m_state.cend(); i != end; ++i) // QMap guarantees sorted
				if (i.value() == Game::Color::Empty)
					m_legalMoves.append(i.key());
		}

		~Node() {
			qDeleteAll(children);
		}
	};

	Game::Color getColorOfPlayer(Players player);
	Node* m_node;
	const QList<QList<int>>& m_graph;
	const Game::Color aiColor;
};