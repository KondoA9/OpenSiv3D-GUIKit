#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class UIView : public UIRect {
		friend GUIKit;

	private:
		Array<UIComponent*> m_components;
		Rect m_scissorRect = Rect(0, 0, 0, 0), m_parentScissorRect = Rect(0, 0, 0, 0);

	public:
		UIView() noexcept :
			UIRect()
		{
			backgroundColor = DynamicColor::Background;
		}

		virtual void release();

		virtual void appendComponent(UIComponent& component);

		virtual ~UIView() {
			release();
		}

		Array<UIComponent*> components() const {
			return m_components;
		}

	protected:
		void updateLayer(const Rect& scissor) override;

		bool updateLayerIfNeeded(const Rect& scissor) override;

		void draw() override;

		void updateInputEvents() override;

	private:
		void updateMouseIntersection() final;

		void updateLayerInvert(const Rect& scissor);

		void updateScissorRect(const Rect& parentScissorRect);
	};
}
