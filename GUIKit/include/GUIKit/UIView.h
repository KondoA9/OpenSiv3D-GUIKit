#pragma once

#include "UIRect.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class UIView : public UIRect {
		friend GUIKit;

	private:
		Array<std::shared_ptr<UIComponent>> m_components;
		Rect m_scissorRect = Rect(0, 0, 0, 0), m_parentScissorRect = Rect(0, 0, 0, 0);

	public:
		UIView() noexcept :
			UIRect(DynamicColor::Background)
		{}

		virtual ~UIView() {
			release();
		};

		virtual void release();

		virtual void appendComponent(const UIComponent& component);

		const Array<std::shared_ptr<UIComponent>>& components() const {
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
