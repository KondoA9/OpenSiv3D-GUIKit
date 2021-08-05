#pragma once

#include "UIRect.h"
#include "UIView.h"
#include "UIToggleButton.h"
#include "GUIFactory.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UITabView : public UIRect {
	private:
		UIView& ui_tabChipsView = GUIFactory::Create<UIView>();
		UIView& ui_tabView = GUIFactory::Create<UIView>();

	public:
		explicit UITabView() noexcept :
			UIRect()
		{}

		virtual ~UITabView() {}

		void appendTab(const String& name, UIView& view);

	protected:
		void initialize() override;

	private:
	};
}
