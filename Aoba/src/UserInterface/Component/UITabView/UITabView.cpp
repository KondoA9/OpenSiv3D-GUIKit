#include "Aoba/UITabView.hpp"

#include "Aoba/PixelUnit.hpp"

namespace s3d::aoba {
	void UITabView::initialize() {
		UIView::initialize();

		ui_tabSelectorView.drawFrame = true;
		ui_tabSelectorView.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
		ui_tabSelectorView.setConstraint(LayerDirection::Height, 30_px);
		ui_tabSelectorView.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_tabSelectorView.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

		ui_tabView.setConstraint(LayerDirection::Top, ui_tabSelectorView, LayerDirection::Bottom);
		ui_tabView.setConstraint(LayerDirection::Bottom, *this, LayerDirection::Bottom);
		ui_tabView.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_tabView.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

		registerInputEvent(Event::Component::TabView::Switched(this, false));
	}

	void UITabView::setTab(size_t index) {
		if (m_tabs.size() > index && index != m_tabIndex) {
			m_tabIndex = index;

			for (auto& tab : m_tabs) {
				tab.hide();
			}

			m_tabs[index].show();

			registerInputEvent(Event::Component::TabView::Switched(this, false));
		}
	}

	void UITabView::setTabEnabled(size_t index, bool enabled) {
		m_tabs[index].enabled = enabled;
	}

	void UITabView::initializeTab(const String& name, UIButton& selector, UIView& view) {
		const size_t index = m_tabs.size();

		selector.setText(name);
		selector.setConstraint(LayerDirection::Top, ui_tabSelectorView, LayerDirection::Top);
		selector.setConstraint(LayerDirection::Bottom, ui_tabSelectorView, LayerDirection::Bottom);
		selector.setConstraint(LayerDirection::Left, [this, index] {
			return ui_tabSelectorView.layer().left() + index * ui_tabSelectorView.layer().width() / ui_tabSelectorView.componentsCount();
			});
		selector.setConstraint(LayerDirection::Width, [this] {
			return ui_tabSelectorView.layer().width() / ui_tabSelectorView.componentsCount();
			});

		selector.addEventListener<Event::Mouse::LeftDown>([this, index] {
			if (m_tabs[index].enabled) {
				setTab(index);
			}
			});

		view.setConstraint(LayerDirection::Top, ui_tabView, LayerDirection::Top);
		view.setConstraint(LayerDirection::Bottom, ui_tabView, LayerDirection::Bottom);
		view.setConstraint(LayerDirection::Left, ui_tabView, LayerDirection::Left);
		view.setConstraint(LayerDirection::Right, ui_tabView, LayerDirection::Right);

		Tab tab = {
			.index = index,
			.view = view,
			.selector = selector
		};

		if (tab.index == m_tabIndex) {
			tab.show();
		}
		else {
			tab.hide();
		}

		m_tabs.push_back(tab);
	}
}
