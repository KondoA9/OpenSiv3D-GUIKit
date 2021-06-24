#include <GUIKit/UIButton.h>
#include <GUIKit/UnifiedFont.h>

using namespace s3d::gui;

UIButton::UIButton(const String& title, const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& defaultTextColor) :
	UIRect(defaultColor),
	title(title),
	icon(icon),
	defaultColor(defaultColor),
	textColor(defaultTextColor),
	hoveredColor(hoveredColor),
	defaultTextColor(defaultTextColor)
{
	addEventListener<MouseEvent::Hovered>([](const auto& e) {
		auto self = static_cast<UIButton*>(e.component);
		self->backgroundColor.highlight(self->hoveredColor);
		});

	addEventListener<MouseEvent::UnHovered>([](const auto& e) {
		auto self = static_cast<UIButton*>(e.component);
		self->backgroundColor.lowlight(self->defaultColor);
		});

	addEventListener<MouseEvent::Hovering>([]() {
		Cursor::RequestStyle(CursorStyle::Hand);
		});
}

UIButton::UIButton(
	const String& title,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& defaultTextColor) :
	UIButton(title, Texture(), defaultColor, hoveredColor, defaultTextColor)
{}

UIButton::UIButton(
	const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& defaultTextColor) :
	UIButton(U"", icon, defaultColor, hoveredColor, defaultTextColor)
{}

UIButton::UIButton(
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& defaultTextColor) :
	UIButton(U"", defaultColor, hoveredColor, defaultTextColor)
{}

void UIButton::draw() {
	UIRect::draw();

	UnifiedFont::Get(UnifiedFontStyle::Medium)(title).drawAt(m_rect.center(), textColor);

	if (icon) {
		icon.drawAt(m_rect.center(), textColor);
	}
}