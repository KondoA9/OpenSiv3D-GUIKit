#include <GUIKit/UITabView.hpp>
#include <GUIKit/GUIFactory.hpp>
#include <GUIKit/PixelUnit.hpp>

namespace s3d::gui {
	void UITabView::initialize() {
		UIView::initialize();

		ui_tabSelectorView.drawFrame = true;
		ui_tabSelectorView.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
		ui_tabSelectorView.setConstraint(LayerDirection::Height, 40_px);
		ui_tabSelectorView.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_tabSelectorView.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

		ui_tabView.setConstraint(LayerDirection::Top, ui_tabSelectorView, LayerDirection::Bottom);
		ui_tabView.setConstraint(LayerDirection::Bottom, *this, LayerDirection::Bottom);
		ui_tabView.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_tabView.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

		appendComponent(ui_tabView);
		appendComponent(ui_tabSelectorView);

		registerInputEvent(TabSwitched(this, false));
	}

	void UITabView::appendTab(const String& name, UIView& view) {
		auto tab = createTab(name, view);
		if (tab.index == m_tabIndex) {
			tab.show();
		}
		else {
			tab.hide();
		}

		m_tabs.push_back(tab);

		ui_tabSelectorView.appendComponent(tab.selector);
		ui_tabView.appendComponent(tab.view);
	}

	void UITabView::setTab(size_t index) {
		for (auto& tab : m_tabs) {
			tab.hide();
		}

		if (m_tabs.size() > index) {
			m_tabs[index].show();
			registerInputEvent(TabSwitched(this, false));
		}
	}

	void UITabView::setTabEnabled(size_t index, bool enabled) {
		m_tabs[index].enabled = enabled;
	}

	UITabView::Tab UITabView::createTab(const String& name, UIView& view) {
		const size_t index = m_tabs.size();

		auto& selector = GUIFactory::Create<UIButton>();
		selector.setText(name);

		selector.setConstraint(LayerDirection::Top, ui_tabSelectorView, LayerDirection::Top);
		selector.setConstraint(LayerDirection::Bottom, ui_tabSelectorView, LayerDirection::Bottom);
		selector.setConstraint(LayerDirection::Left, [this, index] {
			return ui_tabSelectorView.layer().left + index * ui_tabSelectorView.layer().width / ui_tabSelectorView.componentsCount();
			});
		selector.setConstraint(LayerDirection::Width, [this] {
			return ui_tabSelectorView.layer().width / ui_tabSelectorView.componentsCount();
			});

		selector.addEventListener<MouseEvent::LeftDown>([this, index] {
			if (m_tabs[index].enabled) {
				setTab(index);
			}
			});

		view.setConstraint(LayerDirection::Top, ui_tabView, LayerDirection::Top);
		view.setConstraint(LayerDirection::Bottom, ui_tabView, LayerDirection::Bottom);
		view.setConstraint(LayerDirection::Left, ui_tabView, LayerDirection::Left);
		view.setConstraint(LayerDirection::Right, ui_tabView, LayerDirection::Right);

		return {
			.index = index,
			.selector = selector,
			.view = view
		};
	}
}
