#include <Aoba/UICheckBox.hpp>

namespace s3d::aoba {
	void UICheckBox::initialize() {
		UIButton::initialize();

		defaultColor = DynamicColor::BackgroundSecondary;
		hoveredColor = DynamicColor::BackgroundSecondary;
		defaultTextColor = DynamicColor::Text;

		drawFrame = true;

		setCornerRadius(3_px);

		addEventListener<MouseEvent::LeftDown>([this] {
			setChecked(!m_checked);
			}, true);
	}

	void UICheckBox::setChecked(bool checked) {
		m_checked = checked;
		setIcon(m_checked ? m_checkdIcon : Texture());
	}
}
