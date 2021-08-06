#include <GUIKit/Page.h>

namespace s3d::gui {
	Page::Page(const String& identifier) :
		m_identifier(identifier) {
		m_view.setConstraint(LayerDirection::Top);
		m_view.setConstraint(LayerDirection::Bottom, [] {return Window::ClientHeight(); });
		m_view.setConstraint(LayerDirection::Left);
		m_view.setConstraint(LayerDirection::Right, [] {return Window::ClientWidth(); });
	}
}
