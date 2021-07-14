#include <GUIKit/UISlider.h>
#include <GUIKit/PixelUnit.h>

using namespace s3d::gui;

GUICreateInputEvent(Sliding);

UISlider::UISlider(const String& label, UnifiedFontStyle style, TextDirection direction, const ColorTheme& _backgroundColor) :
	UIView(_backgroundColor),
	ui_text(UIText(label, style, direction))
{}

void UISlider::initialize() {
	addEventListener<Sliding>([this] {
		const double pre = m_value;
		m_value = Clamp(m_min + (m_max - m_min) * (Cursor::Pos().x - m_layer.left) / m_layer.width, m_min, m_max);

		if (pre != m_value) {
			requestToUpdateLayer();
			if (m_valueChangedHandler) {
				m_valueChangedHandler(m_value);
			}
		}
		}, true);

	addEventListener<MouseEvent::Hovered>([this] {
		ui_handle.backgroundColor.highlight(DynamicColor::DefaultBlue);
		}, true);

	addEventListener<MouseEvent::UnHovered>([this] {
		ui_handle.backgroundColor.lowlight(DynamicColor::Background);
		}, true);

	addEventListener<MouseEvent::Hovering>([] {
		Cursor::RequestStyle(CursorStyle::Hand);
		}, true);

	const double h = 5.0_px;
	ui_railLeft.drawFrame = true;
	ui_railLeft.backgroundColor = DynamicColor::DefaultBlue;
	ui_railLeft.penetrateMouseEvent = true;
	ui_railLeft.setConstraint(LayerDirection::CenterY, ui_handle, LayerDirection::CenterY);
	ui_railLeft.setConstraint(LayerDirection::Height, h);
	ui_railLeft.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	ui_railLeft.setConstraint(LayerDirection::Right, ui_handle, LayerDirection::CenterX);

	ui_railRight.drawFrame = true;
	ui_railRight.backgroundColor = DynamicColor::BackgroundSecondary;
	ui_railRight.penetrateMouseEvent = true;
	ui_railRight.setConstraint(LayerDirection::CenterY, ui_handle, LayerDirection::CenterY);
	ui_railRight.setConstraint(LayerDirection::Height, h);
	ui_railRight.setConstraint(LayerDirection::Left, ui_handle, LayerDirection::CenterX);
	ui_railRight.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

	const double r = 12_px;
	ui_handle.drawFrame = true;
	ui_handle.backgroundColor = DynamicColor::Background;
	ui_handle.penetrateMouseEvent = true;
	ui_handle.setConstraint(LayerDirection::Top, *this, LayerDirection::CenterY);
	ui_handle.setConstraint(LayerDirection::Height, r);
	ui_handle.setConstraint(LayerDirection::CenterX, [this] {
		return m_layer.left + m_layer.width * (m_value - m_min) / (m_max - m_min);
		});
	ui_handle.setConstraint(LayerDirection::Width, r);

	ui_text.penetrateMouseEvent = true;
	ui_text.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
	ui_text.setConstraint(LayerDirection::Bottom, ui_handle, LayerDirection::Top);
	ui_text.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	ui_text.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

	appendComponent(ui_railLeft);
	appendComponent(ui_railRight);
	appendComponent(ui_handle);
	appendComponent(ui_text);

	UIView::initialize();
}

void UISlider::updateInputEvents() {
	if (mouseCondition().left.down) {
		m_sliding = true;
	}

	if (m_sliding) {
		if (MouseL.up()) {
			m_sliding = false;
		}
		else {
			registerInputEvent(Sliding(this));
		}
	}

	UIView::updateInputEvents();
}
