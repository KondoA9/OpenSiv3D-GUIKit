#pragma once

#include "ColorTheme.h"

namespace s3d::gui::DynamicColor {
	constexpr auto Background = ColorTheme(Color(255, 255, 255), Color(25, 25, 25));
	constexpr auto BackgroundSecondary = ColorTheme(Color(235, 235, 240), Color(65, 65, 65));

	constexpr auto Separator = ColorTheme(Color(180, 180, 180), Color(80, 80, 80));

	constexpr auto Text = ColorTheme(Color(0, 0, 0), Color(255, 255, 255));

	constexpr auto ButtonDefault = ColorTheme(Color(82, 70, 255));
	constexpr auto ButtonPushed = ColorTheme(Color(69, 57, 229));

	constexpr auto DefaultBlue = ColorTheme(Color(40, 150, 255));
	constexpr auto DefaultGreen = ColorTheme(Color(0, 212, 85));
	constexpr auto DefaultRed = ColorTheme(Color(255, 25, 80));

	constexpr auto Clear = ColorTheme(Color(0, 0, 0, 0));
}