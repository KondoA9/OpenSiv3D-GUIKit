#include <GUIKit/UICheckBox.h>

using namespace s3d::gui;

void UICheckBox::initialize() {
	addEventListener<MouseEvent::LeftDown>([this] {
		setChecked(!m_checked);
		}, true);

	drawFrame = true;
	setCornerRadius(5_px);

	UIButton::initialize();
}

void UICheckBox::setChecked(bool checked) {
	m_checked = checked;
	setIcon(m_checked ? m_checkdIcon : Texture());
}
