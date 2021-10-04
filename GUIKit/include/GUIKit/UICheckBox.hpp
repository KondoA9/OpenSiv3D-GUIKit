#pragma once

#include "UIButton.hpp"
#include "UIText.hpp"
#include "PixelUnit.hpp"

namespace s3d::gui {
	class UICheckBox : public UIButton {
	private:
		const Texture m_checkdIcon = Texture(Icon(0xf00c), 13_px);

		bool m_checked = false;

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
