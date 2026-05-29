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
	// TO DO: Make node QMap
	Tree(Game::Color currentPlayer, const QList<QList<int>>& connections, Tree* parent = nullptr) 
		: m_currentPlayer(currentPlayer), m_graph(connections), parent(parent), m_depth(0) {} // Empty tree
	Tree(QList<lwHexagon> node, Game::Color currentPlayer, const QList<QList<int>>& connections) 
		: m_state(node), m_currentPlayer(currentPlayer), m_graph(connections), parent(nullptr), m_depth(0) {} // Root tree
	Tree(QList<lwHexagon> node,  Game::Color currentPlayer, Tree* parent) 
		: m_state(node), m_currentPlayer(currentPlayer), parent(parent), m_graph(parent->getConnections()), m_depth(parent->getDepth() + 1), m_uct(0) { // Child constructor
		for (auto [i, c] : m_state)
			if (c == Game::Color::Empty)
				m_legalMoves.append(i);
	}
	int getDepth() const;
	Game::Color getCurrentPlayer() const;
	QList<QList<int>> getConnections() const;
	
	int mcts();
	static bool haveWon(Game::Color who, QMap<int,Game::Color>& state, const QList<QList<int>>& m_graph);

signals:

private:
	QList<lwHexagon> m_state; // Current board state
	QList<int> m_legalMoves;
	const QList<QList<int>>& m_graph;
	const int m_depth;
	const Game::Color m_currentPlayer;
	int m_win = 0;
	int m_vis = 0;
	double m_uct; // Upper Confidence Bound 1 applied to trees

	const Tree* parent;
	QList<Tree*> children;

	bool runSimulation();
};