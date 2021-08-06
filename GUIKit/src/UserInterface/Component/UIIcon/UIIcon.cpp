#include <GUIKit/UIIcon.hpp>

namespace s3d::gui {
	void UIIcon::draw() {
		UIRect::draw();

		if (rotate) {
			m_icon.rotated(m_angle).drawAt(rect().center(), iconColor);
			m_angle += rotationAnglerVelocity * Scene::DeltaTime();
		}
		else {
			m_icon.drawAt(rect().center(), iconColor);
		}
	}
}
