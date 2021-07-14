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
	textColor(defaultTextColor),
	defaultColor(defaultColor),
	defaultTextColor(defaultTextColor),
	hoveredColor(hoveredColor)
{}

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

void UIButton::initialize() {
	addEventListener<MouseEvent::Hovered>([this] {
		backgroundColor.highlight(hoveredColor);
		}, true);

	addEventListener<MouseEvent::UnHovered>([this] {
		backgroundColor.lowlight(defaultColor);
		}, true);

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::Hand);
		}, true);

	UIRect::initialize();
}

void UIButton::draw() {
	UIRect::draw();

	UnifiedFont::Get(UnifiedFontStyle::Medium)(title).drawAt(rect().center(), textColor);

	if (icon) {
		icon.drawAt(rect().center(), textColor);
	}
}
