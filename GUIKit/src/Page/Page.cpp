#include <GUIKit/Page.hpp>

namespace s3d::gui {
	Page::Page(const String& identifier) :
		m_identifier(identifier) {
		m_view.setConstraint(LayerDirection::Top);
		m_view.setConstraint(LayerDirection::Bottom, [] {
			const auto size = Window::GetState().frameBufferSize;
			return size.y;
			});
		m_view.setConstraint(LayerDirection::Left);
		m_view.setConstraint(LayerDirection::Right, [] {
			const auto size = Window::GetState().frameBufferSize;
			return size.x;
			});
	}
}
