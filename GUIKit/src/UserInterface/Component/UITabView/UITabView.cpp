#include <GUIKit/UITabView.h>
#include <GUIKit/GUIFactory.h>
#include <GUIKit/PixelUnit.h>

namespace s3d::gui {
	void UITabView::initialize() {
		UIRect::initialize();

		ui_tabChipsView.setConstraint(LayerDirection::Top, *this, LayerDirection::Top);
		ui_tabChipsView.setConstraint(LayerDirection::Height, 40_px);
		ui_tabChipsView.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_tabChipsView.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);

		ui_tabView.setConstraint(LayerDirection::Top, ui_tabChipsView, LayerDirection::Bottom);
		ui_tabView.setConstraint(LayerDirection::Bottom, *this, LayerDirection::Bottom);
		ui_tabView.setConstraint(LayerDirection::Left, *this, LayerDirection::Left);
		ui_tabView.setConstraint(LayerDirection::Right, *this, LayerDirection::Right);
	}

	void UITabView::appendTab(const String& name, UIView& view) {
		auto& chip = GUIFactory::Create<UIToggleButton>();
		chip.setTitle(name);
		chip.setFont(UnifiedFontStyle::Small);

		const size_t index = ui_tabChipsView.componentsCount();
		chip.setConstraint(LayerDirection::Top, ui_tabChipsView, LayerDirection::Top);
		chip.setConstraint(LayerDirection::Bottom, ui_tabChipsView, LayerDirection::Bottom);
		chip.setConstraint(LayerDirection::Left, [this, index] {
			return ui_tabChipsView.layer().left + index * ui_tabChipsView.layer().width / ui_tabChipsView.componentsCount();
			});
		chip.setConstraint(LayerDirection::Width, [this] {
			return ui_tabChipsView.layer().width / ui_tabChipsView.componentsCount();
			});

		view.setConstraint(LayerDirection::Top, ui_tabView, LayerDirection::Top);
		view.setConstraint(LayerDirection::Bottom, ui_tabView, LayerDirection::Bottom);
		view.setConstraint(LayerDirection::Left, ui_tabView, LayerDirection::Left);
		view.setConstraint(LayerDirection::Right, ui_tabView, LayerDirection::Right);

		ui_tabChipsView.appendComponent(chip);
		ui_tabView.appendComponent(view);
	}
}
