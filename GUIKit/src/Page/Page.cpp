#include <GUIKit/Page.hpp>
#include <GUIKit/UIView.hpp>
#include <GUIKit/GUIFactory.hpp>

namespace s3d::gui {
	Page::Page(const String& identifier) :
		view(GUIFactory::CreateComponent<UIView>()),
		m_identifier(identifier)
	{
		view.setConstraint(LayerDirection::Top);
		view.setConstraint(LayerDirection::Bottom, [] {
			return Scene::Size().y;
			});
		view.setConstraint(LayerDirection::Left);
		view.setConstraint(LayerDirection::Right, [] {
			return Scene::Size().x;
			});
	}
}
