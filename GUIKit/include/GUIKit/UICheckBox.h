#pragma once

#include "UIButton.h"
#include "UIText.h"
#include "PixelUnit.h"

namespace s3d::gui {
	class UICheckBox : public UIButton {
	private:
		bool m_checked = false;

		const Texture m_checkdIcon = Texture(Icon(0xf00c, 15_px));

	public:
		explicit UICheckBox(
			const ColorTheme& _defaultColor = DynamicColor::BackgroundSecondary,
			const ColorTheme& _defaultTextColor = DynamicColor::Text,
			const ColorTheme& _hoveredColor = DynamicColor::BackgroundSecondary) noexcept :
			UIButton(_defaultColor, _defaultTextColor, _hoveredColor)
		{}

		bool isChecked() const {
			return m_checked;
		}

		void setChecked(bool checked);

	protected:
		void initialize() override;
	};
}
