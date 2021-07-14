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
		explicit UICheckBox(bool checked = false) :
			UIButton(DynamicColor::BackgroundSecondary, DynamicColor::BackgroundSecondary, DynamicColor::Text)
		{
			setChecked(checked);
		}

		bool isChecked() const {
			return m_checked;
		}

		void setChecked(bool checked);

	protected:
		void initialize() override;
	};
}
