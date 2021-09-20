#include <GUIKit/Page.hpp>

namespace s3d::gui {
	Page::Page(const String& identifier) :
		m_identifier(identifier) {
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
