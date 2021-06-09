#include "UIButton.h"
#include "../../Font/UnifiedFont.h"

using namespace s3d::gui;

UIButton::UIButton(const String& title, const Texture& icon,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIRect(defaultColor),
	title(title),
	icon(icon),
	defaultColor(defaultColor),
	hoveredColor(hoveredColor),
	textColor(textColor)
{}

UIButton::UIButton(
	const String& title,
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIButton(title, Texture(), defaultColor, hoveredColor, textColor)
{}

UIButton::UIButton(
	const Texture& _icon,
	const ColorTheme& _defaultColor,
	const ColorTheme& _hoveredColor,
	const ColorTheme& textColor) :
	UIButton(U"", _icon, _defaultColor, _hoveredColor, textColor)
{}

UIButton::UIButton(
	const ColorTheme& defaultColor,
	const ColorTheme& hoveredColor,
	const ColorTheme& textColor) :
	UIButton(U"", defaultColor, hoveredColor, textColor)
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