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
{
	addEventListener<MouseEvent::LeftDown>([](const MouseEvent::LeftDown& e) {
		auto self = static_cast<UIToggleButton*>(e.component);
		self->setEnabled(!self->isEnabled());
		if (!self->isEnabled()) {
			self->backgroundColor.highlight(self->hoveredColor);
		}
		});

	addEventListener<MouseEvent::Hovered>([](const MouseEvent::Hovered& e) {
		auto self = static_cast<UIToggleButton*>(e.component);
		if (!self->isEnabled()) {
			self->backgroundColor.highlight(self->hoveredColor);
		}
		});

	addEventListener<MouseEvent::UnHovered>([](const MouseEvent::UnHovered& e) {
		auto self = static_cast<UIToggleButton*>(e.component);
		if (!self->isEnabled()) {
			self->backgroundColor.lowlight(self->defaultColor);
		}
		});
}

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