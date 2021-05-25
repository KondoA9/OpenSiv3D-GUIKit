#pragma once

#include "../Window/WindowManager.h"
#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIView : public UIRect {
	protected:
		Array<UIComponent*> userInterfaces;

	public:
		UIView(const ColorTheme& _backgroundColor = DynamicColor::background) :
			UIRect(_backgroundColor)
		{
			layer.top.setConstraint();
			layer.left.setConstraint();
			layer.bottom.setConstraint([]() {return Window::ClientHeight(); });
			layer.right.setConstraint([]() {return Window::ClientWidth(); });
		}

		~UIView() {}

		virtual void appendComponent(UIComponent& ui) {
			userInterfaces.push_back(&ui);
		}

		void updateShape() override {
			UIRect::updateShape();
			for (auto& ui : userInterfaces) {
				ui->updateShape();
			}
		}

		bool updateShapeIfNeeded() override {
			bool updated = false;
			updated |= UIRect::updateShapeIfNeeded();
			for (auto& ui : userInterfaces) {
				updated |= ui->updateShapeIfNeeded();
			}

			return updated;
		}

		void draw() override {
			UIRect::draw();
			for (auto& ui : userInterfaces) {
				ui->draw();
			}
		}

		void control() override {
			UIRect::control();
			for (auto& ui : userInterfaces) {
				ui->control();
			}
		}
	};
}