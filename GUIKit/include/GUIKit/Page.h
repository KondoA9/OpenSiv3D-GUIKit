#pragma once

#include "UIView.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class Page {
		friend GUIKit;

	protected:
		UIView m_view = UIView();

	private:
		const String m_identifier;
		bool m_loaded = false;

	public:
		Page(const String& identifier) :
			m_identifier(identifier) {
			m_view.setConstraint(LayerDirection::Top);
			m_view.setConstraint(LayerDirection::Bottom, [] {return Window::ClientHeight(); });
			m_view.setConstraint(LayerDirection::Left);
			m_view.setConstraint(LayerDirection::Right, [] {return Window::ClientWidth(); });
		}

		virtual ~Page() = default;

		// Called when guikit core system loaded the page
		virtual void onLoaded() {}

		// Called before guikit core system draws the page
		virtual void onBeforeAppeared() {}

		// Called when all constraints updated
		virtual void onLayoutCompleted() {}

		// Called after guikit core system draws the page
		virtual void onAfterAppeared() {}

		// Called before guikit core system erases the page
		virtual void onBeforeDisappeared() {}

		// Called after guikit core system erases the page
		virtual void onAfterDisappeared() {}

		// Called when window resized
		virtual void onWindowResized() {}

		// Called when application terminated
		virtual void onAppTerminated() {}
	};
}
