#include <GUIKit/UISlider.h>
#include <GUIKit/PixelUnit.h>

using namespace s3d::gui;

UISlider::UISlider(const String& label, UnifiedFontStyle style, TextDirection direction, const ColorTheme& _backgroundColor) :
	UIView(_backgroundColor),
	m_text(UIText(label, style, direction))
{
	addEventListener<MouseEvent::LeftDragging>([] (const auto& e){
		auto self = static_cast<UISlider*>(e.component);
		if (!self->m_dragging) {
			self->m_dragging = true;
		}
		if (MouseL.up() && self->m_dragging) {
			self->m_dragging = false;
		}

		if (self->m_dragging) {
			const double pre = self->m_value;
			self->m_value = Clamp(self->m_min + (self->m_max - self->m_min) * (Cursor::Pos().x - self->m_layer.left) / self->m_layer.width, self->m_min, self->m_max);

			if (pre != self->m_value) {
				self->requestToUpdateLayer();
				if (self->m_valueChangedHandler) {
					self->m_valueChangedHandler(self->m_value);
				}
				return true;
			}
		}
		});

	addEventListener<MouseEvent::Hovered>([](const auto& e) {
		auto self = static_cast<UISlider*>(e.component);
		self->handle.backgroundColor = DynamicColor::DefaultBlue;
		});

	addEventListener<MouseEvent::UnHovered>([](const auto& e) {
		auto self = static_cast<UISlider*>(e.component);
		self->handle.backgroundColor = DynamicColor::Background;
		});

	addEventListener<MouseEvent::UnHovered>([]() {
		Cursor::RequestStyle(CursorStyle::Hand);
		});
}

void UISlider::updateLayer() {
	if (!m_initialized) {
		initialize();
		m_initialized = true;
	}

	UIView::updateLayer();
}

void UISlider::draw() {
	UIView::draw();
}

void UISlider::initialize() {
	const double h = 5.0_px;
	railLeft.drawFrame = true;
	railLeft.backgroundColor = DynamicColor::DefaultBlue;
	railLeft.setConstraint(LayerDirection::CenterY, handle, LayerDirection::CenterY);
	railLeft.setConstraint(LayerDirection::Height, h);
	railLeft.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	railLeft.setConstraint(LayerDirection::Right, handle, LayerDirection::CenterX);

	railRight.drawFrame = true;
	railRight.backgroundColor = DynamicColor::BackgroundSecondary;
	railRight.setConstraint(LayerDirection::CenterY, handle, LayerDirection::CenterY);
	railRight.setConstraint(LayerDirection::Height, h);
	railRight.setConstraint(LayerDirection::Left, handle, LayerDirection::CenterX);
	railRight.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

	const double r = 12_px;
	handle.drawFrame = true;
	handle.backgroundColor = DynamicColor::Background;
	handle.setConstraint(LayerDirection::Top, *this, LayerDirection::CenterY);
	handle.setConstraint(LayerDirection::Height, r);
	handle.setConstraint(LayerDirection::CenterX, [this] {
		return m_layer.left + m_layer.width * (m_value - m_min) / (m_max - m_min);
		});
	handle.setConstraint(LayerDirection::Width, r);

	m_text.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
	m_text.setConstraint(LayerDirection::Bottom, handle, LayerDirection::Top);
	m_text.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
	m_text.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

	appendComponent(railLeft);
	appendComponent(railRight);
	appendComponent(handle);
	appendComponent(m_text);
}