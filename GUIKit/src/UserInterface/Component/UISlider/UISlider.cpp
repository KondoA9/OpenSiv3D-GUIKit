#include <GUIKit/UISlider.hpp>
#include <GUIKit/PixelUnit.hpp>

GUICreateInputEvent(Sliding);

namespace s3d::gui {
	void UISlider::initialize() {
		const double handleRadius = 6_px;

		addEventListener<Sliding>([this, handleRadius] {
			const double pre = m_value;
			m_value = Clamp(m_min + (m_max - m_min) * (Cursor::Pos().x - (layer().left + handleRadius)) / (layer().width - handleRadius * 2), m_min, m_max);

			if (pre != m_value) {
				requestToUpdateLayer();
				if (m_valueChangedHandler) {
					m_valueChangedHandler(m_value);
				}
			}
			}, true);

		addEventListener<MouseEvent::Hovered>([this] {
			m_ui_handle.backgroundColor.highlight(DynamicColor::DefaultBlue);
			}, true);

		addEventListener<MouseEvent::UnHovered>([this] {
			m_ui_handle.backgroundColor.lowlight(DynamicColor::Background);
			}, true);

		addEventListener<MouseEvent::Hovering>([] {
			Cursor::RequestStyle(CursorStyle::Hand);
			}, true);

		const double h = 3.0_px;
		m_ui_railLeft.drawFrame = true;
		m_ui_railLeft.backgroundColor = DynamicColor::DefaultBlue;
		m_ui_railLeft.penetrateMouseEvent = true;
		m_ui_railLeft.setConstraint(LayerDirection::CenterY, m_ui_handle, LayerDirection::CenterY);
		m_ui_railLeft.setConstraint(LayerDirection::Height, h);
		m_ui_railLeft.setConstraint(LayerDirection::Left, *this, LayerDirection::Left, handleRadius);
		m_ui_railLeft.setConstraint(LayerDirection::Right, m_ui_handle, LayerDirection::CenterX);

		m_ui_railRight.drawFrame = true;
		m_ui_railRight.backgroundColor = DynamicColor::BackgroundSecondary;
		m_ui_railRight.penetrateMouseEvent = true;
		m_ui_railRight.setConstraint(LayerDirection::CenterY, m_ui_handle, LayerDirection::CenterY);
		m_ui_railRight.setConstraint(LayerDirection::Height, h);
		m_ui_railRight.setConstraint(LayerDirection::Left, m_ui_handle, LayerDirection::CenterX);
		m_ui_railRight.setConstraint(LayerDirection::Right, *this, LayerDirection::Right, -handleRadius);

		m_ui_handle.drawFrame = true;
		m_ui_handle.backgroundColor = DynamicColor::Background;
		m_ui_handle.penetrateMouseEvent = true;
		m_ui_handle.setConstraint(LayerDirection::Top, *this, LayerDirection::CenterY);
		m_ui_handle.setConstraint(LayerDirection::Height, handleRadius * 2);
		m_ui_handle.setConstraint(LayerDirection::CenterX, [this, handleRadius] {
			return layer().left + handleRadius + (layer().width - handleRadius * 2) * (m_value - m_min) / (m_max - m_min);
			});
		m_ui_handle.setConstraint(LayerDirection::Width, handleRadius * 2);

		ui_text.penetrateMouseEvent = true;
		ui_text.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
		ui_text.setConstraint(LayerDirection::Bottom, m_ui_handle, LayerDirection::Top);
		ui_text.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_text.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

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
				registerInputEvent(Sliding(this, false));
			}
		}

		UIView::updateInputEvents();
	}
}
