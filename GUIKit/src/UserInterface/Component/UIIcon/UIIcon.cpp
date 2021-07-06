#include <GUIKit/UIIcon.h>

using namespace s3d::gui;

void UIIcon::draw(const Rect& scissor) {
	UIRect::draw(scissor);

	if (rotate) {
		m_icon.rotated(m_angle).drawAt(m_rect.center(), iconColor);
		m_angle += rotationAnglerVelocity * Scene::DeltaTime();
	}
	else {
		m_icon.drawAt(m_rect.center(), iconColor);
	}
}
