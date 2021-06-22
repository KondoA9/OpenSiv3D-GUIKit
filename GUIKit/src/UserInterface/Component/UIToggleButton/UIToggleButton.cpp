#include <GUIKit/UIToggleButton.h>

using namespace s3d::gui;

UIToggleButton::UIToggleButton(const String& title, const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor,
	const ColorTheme& selectedColor,
	const ColorTheme& selectedTextColor) :
	UIButton(title, icon, defaultColor, hoveredColor, textColor),
	selectedColor(selectedColor),
	selectedTextColor(selectedTextColor)
{}

UIToggleButton::UIToggleButton(
	const String& title,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor,
	const ColorTheme& selectedColor,
	const ColorTheme& selectedTextColor) :
	UIToggleButton(title, Texture(), defaultColor, hoveredColor, textColor, selectedColor, selectedTextColor)
{}

UIToggleButton::UIToggleButton(
	const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor,
	const ColorTheme& selectedColor,
	const ColorTheme& selectedTextColor) :
	UIToggleButton(U"", icon, defaultColor, hoveredColor, textColor, selectedColor, selectedTextColor)
{}

UIToggleButton::UIToggleButton(
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor,
	const ColorTheme& selectedColor,
	const ColorTheme& selectedTextColor) :
	UIToggleButton(U"", defaultColor, hoveredColor, textColor, selectedColor, selectedTextColor)
{}

bool UIToggleButton::mouseHovered() {
	if (!m_enabled && UIRect::mouseHovered()) {
		backgroundColor.highlight(hoveredColor);
		return true;
	}
	return false;
}

bool UIToggleButton::mouseUnHovered() {
	if (!m_enabled && UIRect::mouseUnHovered()) {
		backgroundColor.lowlight(defaultColor);
		return true;
	}
	return false;
}

bool UIToggleButton::mouseHovering() {
	if (UIRect::mouseHovering()) {
		Cursor::RequestStyle(CursorStyle::Hand);
		return true;
	}
	return false;
}

bool UIToggleButton::mouseLeftDown() {
	if (UIButton::mouseLeftDown()) {
		m_enabled = !m_enabled;
		if (m_enabled) {
			backgroundColor.highlight(selectedColor);
			textColor.highlight(selectedTextColor);
		}
		else {
			backgroundColor.lowlight(hoveredColor);
			textColor.lowlight(defaultTextColor);
		}
		return true;
	}
	return false;
}