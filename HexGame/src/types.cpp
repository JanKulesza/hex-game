#include "types.h"

QString enumToString(Game::Color color) {
	switch (color) {
	case Game::Color::Red:   return "#f01618";
	case Game::Color::Blue:  return "#006cfd";
	case Game::Color::Empty: return "#141c29";
	default:           return "#141c29";
	}
}