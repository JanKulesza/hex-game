#pragma once

class Game : public QObject {
	Q_OBJECT
		QML_UNCREATABLE("Enum class is uncreatable")
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

constexpr QColor to_string(Game::Color color) {
	switch (color) {
	case Game::Color::Red:   return "#f01618";
	case Game::Color::Blue:  return "#006cfd";
	case Game::Color::Empty: return "#141c29";
	default:           return "#141c29";
	}
}