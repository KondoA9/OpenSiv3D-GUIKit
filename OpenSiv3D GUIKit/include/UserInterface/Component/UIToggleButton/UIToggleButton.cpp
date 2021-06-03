#include "UIToggleButton.h"

using namespace s3d::gui;

UIToggleButton::UIToggleButton(const String& title, const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIButton(title, icon, defaultColor, hoveredColor, textColor)
{}

UIToggleButton::UIToggleButton(
	const String& title,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIToggleButton(title, Texture(), defaultColor, hoveredColor, textColor)
{}

UIToggleButton::UIToggleButton(
	const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIToggleButton(U"", icon, defaultColor, hoveredColor, textColor)
{}

UIToggleButton::UIToggleButton(
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIToggleButton(U"", defaultColor, hoveredColor, textColor)
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
		return true;
	}
	return false;
}