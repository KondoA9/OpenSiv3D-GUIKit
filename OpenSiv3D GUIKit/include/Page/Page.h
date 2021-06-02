#pragma once

#include "../UserInterface/Component/UIView/UIView.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;

	class Page {
	public:
		const String identifier;
		GUIKit* guikit;
		UIView view;
		bool loaded = false;

	public:
		Page(const String& _identifier) :
			identifier(_identifier) {
			view = gui::UIView();
			view.setConstraint(LayerDirection::Top);
			view.setConstraint(LayerDirection::Bottom, [] {return Window::ClientHeight(); });
			view.setConstraint(LayerDirection::Left);
			view.setConstraint(LayerDirection::Right, [] {return Window::ClientWidth(); });
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
	};
}