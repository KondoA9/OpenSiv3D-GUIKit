#include <Aoba/UIIcon.hpp>

namespace s3d::aoba {
	void UIIcon::update() {
		UIRect::update();

		if (rotate) {
			m_angle += rotationAnglerVelocity * Scene::DeltaTime();
		}
	}

	void UIIcon::draw() const {
		UIRect::draw();

		if (rotate) {
			m_icon.rotated(m_angle).drawAt(rect().center(), iconColor);
		}
		else {
			m_icon.drawAt(rect().center(), iconColor);
		}
	}
}
