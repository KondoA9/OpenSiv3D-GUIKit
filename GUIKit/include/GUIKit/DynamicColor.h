#pragma once

#include "ColorTheme.h"

namespace s3d::gui::DynamicColor {
	constexpr auto Background = ColorTheme(Color(250, 250, 250), Color(33, 33, 33));
	constexpr auto BackgroundSecondary = ColorTheme(Color(236, 239, 241), Color(66, 66, 66));
	constexpr auto BackgroundTertiary = ColorTheme(Color(207, 216, 220), Color(97, 97, 97));

	constexpr auto BackgroundHovered = ColorTheme(Color(243, 245, 246), Color(48, 48, 48));

	constexpr auto Separator = ColorTheme(Color(189, 189, 189), Color(97, 97, 97));

	constexpr auto Text = ColorTheme(Color(33, 33, 33), Color(250, 250, 250));

	constexpr auto ButtonDefault = ColorTheme(Color(112, 100, 255), Color(82, 70, 255));
	constexpr auto ButtonPushed = ColorTheme(Color(95, 80, 229), Color(69, 57, 229));

	constexpr auto DefaultRed = ColorTheme(Color(244, 67, 54));
	constexpr auto DefaultPink = ColorTheme(Color(233, 30, 99));
	constexpr auto DefaultPurple = ColorTheme(Color(156, 39, 176));
	constexpr auto DefaultDeepPurple = ColorTheme(Color(103, 58, 183));
	constexpr auto DefaultIndigo = ColorTheme(Color(63, 81, 181));
	constexpr auto DefaultBlue = ColorTheme(Color(33, 150, 243));
	constexpr auto DefaultLightBlue = ColorTheme(Color(3, 169, 244));
	constexpr auto DefaultCyan = ColorTheme(Color(0, 188, 212));
	constexpr auto DefaultTeal = ColorTheme(Color(0, 150, 136));
	constexpr auto DefaultGreen = ColorTheme(Color(76, 175, 80));
	constexpr auto DefaultLightGreen = ColorTheme(Color(139, 195, 74));
	constexpr auto DefaultLime = ColorTheme(Color(205, 220, 57));
	constexpr auto DefaultYellow = ColorTheme(Color(255, 235, 59));
	constexpr auto DefaultAmber = ColorTheme(Color(255, 193, 7));
	constexpr auto DefaultOrange = ColorTheme(Color(255, 152, 0));
	constexpr auto DefaultDeepOrange = ColorTheme(Color(255, 87, 34));
	constexpr auto DefaultBrown = ColorTheme(Color(121, 85, 72));
	constexpr auto DefaultGray = ColorTheme(Color(158, 158, 158));
	constexpr auto DefaultBlueGray = ColorTheme(Color(96, 125, 139));

	constexpr auto Clear = ColorTheme(Color(0, 0, 0, 0));
}
