#include "Aoba/Component/UIIcon.hpp"

namespace s3d::aoba {
    void UIIcon::initialize() {
        UIRect::initialize();

        backgroundColor = DynamicColor::Clear;
    }

    void UIIcon::update() {
        UIRect::update();

        if (rotate) {
            m_angle += rotationAnglerVelocity * Scene::DeltaTime();
        }
    }

    void UIIcon::draw() const {
        UIRect::draw();

        if (rotate) {
            m_icon.rotated(m_angle).drawAt(layer().center(), iconColor);
        } else {
            m_icon.drawAt(layer().center(), iconColor);
        }
    }
}
