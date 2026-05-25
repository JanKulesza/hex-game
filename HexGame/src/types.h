enum class Color {
	Red,
	Blue,
	Empty
};

constexpr QColor to_string(Color color) {
    switch (color) {
        case Color::Red:   return "#f01618";
        case Color::Blue:  return "#006cfd";
        case Color::Empty: return "#141c29";
        default:           return "#141c29";
    }
}