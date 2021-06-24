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
		self->m_enabled = !self->m_enabled;
		if (self->m_enabled) {
			self->backgroundColor.highlight(self->selectedColor);
			self->textColor.highlight(self->selectedTextColor);
		}
		else {
			self->backgroundColor.lowlight(self->hoveredColor);
			self->textColor.lowlight(self->defaultTextColor);
		}
		});

	addEventListener<MouseEvent::Hovered>([](const MouseEvent::Hovered& e) {
		auto self = static_cast<UIToggleButton*>(e.component);
		if (!self->m_enabled) {
			self->backgroundColor.highlight(self->hoveredColor);
		}
		else {
			self->backgroundColor.highlight(self->selectedColor);
		}
		});

	addEventListener<MouseEvent::UnHovered>([](const MouseEvent::UnHovered& e) {
		auto self = static_cast<UIToggleButton*>(e.component);
		if (!self->m_enabled) {
			self->backgroundColor.lowlight(self->defaultColor);
		}
		else {
			self->backgroundColor.highlight(self->selectedColor);
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