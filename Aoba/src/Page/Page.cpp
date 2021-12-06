#include <Aoba/Page.hpp>
#include <Aoba/UIView.hpp>
#include <Aoba/Factory.hpp>

namespace s3d::aoba {
	Page::Page(const String& identifier) :
		view(AobaFactory::CreateComponent<UIView>()),
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
