#pragma once

#include "ColorTheme.h"

namespace s3d::gui::DynamicColor {
	constexpr auto background = ColorTheme(Color(255, 255, 255), Color(55, 55, 55));
	constexpr auto backgroundSecondary = ColorTheme(Color(245, 245, 250), Color(85, 85, 85));

	constexpr auto text = ColorTheme(Color(0, 0, 0), Color(255, 255, 255));

	constexpr auto buttonDefault = ColorTheme(Color(82, 70, 255));
	constexpr auto buttonPushed = ColorTheme(Color(69, 57, 229));

	constexpr auto clear = ColorTheme(Color(0, 0, 0, 0));
}