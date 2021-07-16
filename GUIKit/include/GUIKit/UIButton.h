#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIButton : public UIRect {
	public:
		String title = U"";
		Texture icon;
		ColorTheme textColor = Palette::White, defaultColor = DynamicColor::ButtonDefault, defaultTextColor = Palette::White, hoveredColor = DynamicColor::ButtonPushed;

	public:
		UIButton() noexcept :
			UIRect()
		{
			backgroundColor = defaultColor;
		}

		void setTitle(const String& _title) {
			title = _title;
		}

		void setIcon(const Texture& _icon) {
			icon = _icon;
		}

	protected:
		void initialize() override;

		void draw() override;
	};
}
