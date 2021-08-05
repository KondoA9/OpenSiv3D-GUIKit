#pragma once

#include "UIRect.h"
#include "UIView.h"
#include "UIToggleButton.h"
#include "GUIFactory.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UITabView : public UIView {
		struct Tab {
			size_t index;
			UIToggleButton& selector;
			UIView& view;

			void show() {
				selector.setEnabled(true);
				view.hidden = false;
			}

			void hide() {
				selector.setEnabled(false);
				view.hidden = true;
			}
		};

	private:
		UIView& ui_tabSelectorView = GUIFactory::Create<UIView>();
		UIView& ui_tabView = GUIFactory::Create<UIView>();

		Array<Tab> m_tabs;
		size_t m_tabIndex = 0;

	public:
		explicit UITabView() noexcept :
			UIView()
		{}

		virtual ~UITabView() {}

		void appendTab(const String& name, UIView& view);

	protected:
		void initialize() override;

	private:
		Tab createTab(const String& name, UIView& view);
	};
}
