#pragma once

#include "ColorTheme.h"

namespace s3d::gui::DynamicColor {
	constexpr auto Background = ColorTheme(Color(255, 255, 255), Color(55, 55, 55));
	constexpr auto BackgroundSecondary = ColorTheme(Color(245, 245, 250), Color(85, 85, 85));

	constexpr auto Text = ColorTheme(Color(0, 0, 0), Color(255, 255, 255));

	constexpr auto ButtonDefault = ColorTheme(Color(82, 70, 255));
	constexpr auto ButtonPushed = ColorTheme(Color(69, 57, 229));

	constexpr auto Clear = ColorTheme(Color(0, 0, 0, 0));
}