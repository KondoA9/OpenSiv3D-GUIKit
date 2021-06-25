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

void UIToggleButton::initialize() {
	addEventListener<MouseEvent::LeftDown>([this] {
		m_enabled = !m_enabled;
		if (m_enabled) {
			backgroundColor.highlight(selectedColor);
			textColor.highlight(selectedTextColor);
		}
		else {
			backgroundColor.lowlight(hoveredColor);
			textColor.lowlight(defaultTextColor);
		}
		}, true);

	addEventListener<MouseEvent::Hovered>([this] {
		if (!m_enabled) {
			backgroundColor.highlight(hoveredColor);
		}
		else {
			backgroundColor.highlight(selectedColor);
		}
		}, true);

	addEventListener<MouseEvent::UnHovered>([this] {
		if (!m_enabled) {
			backgroundColor.lowlight(defaultColor);
		}
		else {
			backgroundColor.highlight(selectedColor);
		}
		}, true);

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::Hand);
		}, true);

	UIRect::initialize();
}