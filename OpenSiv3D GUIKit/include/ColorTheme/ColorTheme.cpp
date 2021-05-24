#include "ColorTheme.h"

namespace s3d::gui {
	ColorMode currentMode;
	double t = 0.0;

	Color ColorTheme::color() const {
		const auto r = (static_cast<double>(dark.r) - static_cast<double>(light.r)) * t;
		const auto g = (static_cast<double>(dark.g) - static_cast<double>(light.g)) * t;
		const auto b = (static_cast<double>(dark.b) - static_cast<double>(light.b)) * t;
		const auto a = (static_cast<double>(dark.a) - static_cast<double>(light.a)) * t;
		return Color(static_cast<uint32>(light.r + r), static_cast<uint32>(light.g + g), static_cast<uint32>(light.b + b), static_cast<uint32>(light.a + a));
	}

	void ColorTheme::setColorMode(ColorMode mode) {
		currentMode = mode;
	}

	ColorMode ColorTheme::colorMode() {
		return currentMode;
	}
	
	void ColorTheme::animate(double _t) {
		t = _t;
	}
}