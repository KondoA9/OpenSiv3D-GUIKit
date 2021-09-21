#include <GUIKit/Page.hpp>
#include <GUIKit/UIView.hpp>
#include <GUIKit/GUIFactory.hpp>

namespace s3d::gui {
	Page::Page(const String& identifier) :
		m_identifier(identifier),
		m_view(GUIFactory::CreateComponent<UIView>())
	{
		m_view.setConstraint(LayerDirection::Top);
		m_view.setConstraint(LayerDirection::Bottom, [] {
			return Scene::Size().y;
			});
		m_view.setConstraint(LayerDirection::Left);
		m_view.setConstraint(LayerDirection::Right, [] {
			return Scene::Size().x;
			});
	}
}
