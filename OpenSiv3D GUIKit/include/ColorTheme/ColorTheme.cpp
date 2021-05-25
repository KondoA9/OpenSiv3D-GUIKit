#include "ColorTheme.h"

namespace s3d::gui {
	ColorMode currentMode;
	double t = 0.0;

	Color ColorTheme::color() const {
		if (m_isTransition) {
			m_transitionTimer += Scene::DeltaTime();
			const double k = m_transitionTimer / m_transitionTime;
			if (k > 1.0) {
				light = m_transitionLight;
				dark = m_transitionDark;
				m_isTransition = false;
				return currentMode == ColorMode::Light ? light : dark;
			}
			const auto color = currentMode == ColorMode::Light ? light : dark;
			const auto transitionColor = currentMode == ColorMode::Light ? m_transitionLight : m_transitionDark;
			const auto r = (static_cast<double>(transitionColor.r) - static_cast<double>(color.r)) * k;
			const auto g = (static_cast<double>(transitionColor.g) - static_cast<double>(color.g)) * k;
			const auto b = (static_cast<double>(transitionColor.b) - static_cast<double>(color.b)) * k;
			const auto a = (static_cast<double>(transitionColor.a) - static_cast<double>(color.a)) * k;
			return Color(static_cast<uint32>(color.r + r), static_cast<uint32>(color.g + g), static_cast<uint32>(color.b + b), static_cast<uint32>(color.a + a));
		}
		else {
			const auto r = (static_cast<double>(dark.r) - static_cast<double>(light.r)) * t;
			const auto g = (static_cast<double>(dark.g) - static_cast<double>(light.g)) * t;
			const auto b = (static_cast<double>(dark.b) - static_cast<double>(light.b)) * t;
			const auto a = (static_cast<double>(dark.a) - static_cast<double>(light.a)) * t;
			return Color(static_cast<uint32>(light.r + r), static_cast<uint32>(light.g + g), static_cast<uint32>(light.b + b), static_cast<uint32>(light.a + a));
		}
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