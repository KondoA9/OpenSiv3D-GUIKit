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

void UIButton::draw() {
	UIRect::draw();

	UnifiedFont::Get(UnifiedFontStyle::Medium)(title).drawAt(m_rect.center(), textColor);

	if (icon) {
		icon.drawAt(m_rect.center(), textColor);
	}
}

bool UIButton::mouseHovered() {
	if (UIRect::mouseHovered()) {
		backgroundColor.highlight(hoveredColor);
		return true;
	}
	return false;
}

bool UIButton::mouseUnHovered() {
	if (UIRect::mouseUnHovered()) {
		backgroundColor.lowlight(defaultColor);
		return true;
	}
	return false;
}

bool UIButton::mouseHovering() {
	if (UIRect::mouseHovering()) {
		Cursor::RequestStyle(CursorStyle::Hand);
		return true;
	}
	return false;
}