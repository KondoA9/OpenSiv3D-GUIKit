#pragma once

#include "../Window/WindowManager.h"
#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIView : public UIRect {
	protected:
		Array<UIComponent*> userInterfaces;

	public:
		UIView(const ColorTheme& _backgroundColor = DynamicColor::Background) :
			UIRect(_backgroundColor)
		{
			m_layer.top.setConstraint();
			m_layer.left.setConstraint();
			m_layer.bottom.setConstraint([]() {return Window::ClientHeight(); });
			m_layer.right.setConstraint([]() {return Window::ClientWidth(); });
		}

		~UIView() {}

		virtual void appendComponent(UIComponent& ui) {
			userInterfaces.push_back(&ui);
		}

		void updateLayer() override {
			UIRect::updateLayer();
			for (auto& ui : userInterfaces) {
				ui->updateLayer();
			}
		}

		bool updateLayerIfNeeded() override {
			bool updated = false;
			updated |= UIRect::updateLayerIfNeeded();
			for (auto& ui : userInterfaces) {
				updated |= ui->updateLayerIfNeeded();
			}

			return updated;
		}

		void draw() override {
			UIRect::draw();
			for (auto& ui : userInterfaces) {
				ui->draw();
			}
		}

		void updateMouseEvent() override {
			UIRect::updateMouseEvent();
			for (auto& ui : userInterfaces) {
				ui->updateMouseEvent();
			}
		}
	};
}