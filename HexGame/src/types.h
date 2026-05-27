#pragma once
#include <QObject>
#include <QString>
#include <QtQml/qqml.h>

class Game : public QObject {
	Q_OBJECT
	QML_UNCREATABLE("Enum class is uncreatable");
public:
	enum Difficulty {
		Easy,
		Medium,
		Hard,
		Expert
	};
	enum Color {
		Red,
		Blue,
		Empty
	};
	Q_ENUM(Difficulty)
	Q_ENUM(Color)
};

QString enumToString(Game::Color color);