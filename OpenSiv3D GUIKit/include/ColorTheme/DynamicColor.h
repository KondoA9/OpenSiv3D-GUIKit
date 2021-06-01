#pragma once

#include "ColorTheme.h"

namespace s3d::gui::DynamicColor {
	constexpr auto Background = ColorTheme(Color(255, 255, 255), Color(25, 25, 25));
	constexpr auto BackgroundSecondary = ColorTheme(Color(240, 240, 245), Color(55, 55, 55));

	constexpr auto Separator = ColorTheme(Color(180, 180, 180), Color(80, 80, 80));

	constexpr auto Text = ColorTheme(Color(0, 0, 0), Color(255, 255, 255));

	constexpr auto ButtonDefault = ColorTheme(Color(82, 70, 255));
	constexpr auto ButtonPushed = ColorTheme(Color(69, 57, 229));

	constexpr auto DefaultBlue = ColorTheme(Color(40, 150, 255));
	constexpr auto Clear = ColorTheme(Color(0, 0, 0, 0));
}