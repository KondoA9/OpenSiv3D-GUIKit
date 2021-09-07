#pragma once

#include "UIRect.hpp"
#include "UIView.hpp"
#include "UIToggleButton.hpp"
#include "GUIFactory.hpp"

#include <Siv3D.hpp>

namespace s3d::gui {
	// Called when tab switched or tab view loaded.
	GUICreateInputEvent(TabSwitched);

	class UITabView : public UIView {
		struct Tab {
			size_t index;
			UIButton& selector;
			UIView& view;

			bool enabled = true;

			void show() {
				selector.setFont(UnifiedFontStyle::DefaultBold);
				view.hidden = false;
			}

			void hide() {
				selector.setFont(UnifiedFontStyle::DefaultLight);
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

		size_t index() const {
			return m_tabIndex;
		}

		void appendTab(const String& name, UIView& view);

		void setTab(size_t index);

		void setTabEnabled(size_t index, bool enabled);

	protected:
		void initialize() override;

	private:
		Tab createTab(const String& name, UIView& view);
	};
}
