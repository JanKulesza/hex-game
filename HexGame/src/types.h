#pragma once
#include <QObject>
#include <QString>
#include <QtQml/qqml.h>

class Game : public QObject {
	Q_OBJECT
	QML_UNCREATABLE("Enum class is uncreatable");
public:
	enum class Difficulty: uint8_t {
		Easy,
		Medium,
		Hard,
		Expert
	};
	enum class Color: uint8_t {
		Red,
		Blue,
		Empty
	};
	Q_ENUM(Difficulty)
	Q_ENUM(Color)
};

QString enumToString(Game::Color color);