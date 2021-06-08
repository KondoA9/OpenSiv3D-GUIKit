#include "UIIcon.h"

using namespace s3d::gui;

void UIIcon::draw() {
	if (!drawable()) {
		return;
	}

	UIRect::draw();

	if (rotate) {
		m_icon.rotated(angle).drawAt(m_rect.center(), iconColor);
		angle += rotationAnglerVelocity * Scene::DeltaTime();
	}
	else {
		m_icon.drawAt(m_rect.center(), iconColor);
	}
}