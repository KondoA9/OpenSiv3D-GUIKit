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
		}

		virtual ~Page() = default;

		// Called when guikit core system loaded the page
		virtual void onLoaded() {}

		// Called before guikit core system draws the page
		virtual void onBeforeAppeared() {}

		// Called after guikit core system draws the page
		virtual void onAfrerAppeared() {}

		// Called before guikit core system erases the page
		virtual void onBeforeDisappeared() {}

		// Called after guikit core system erases the page
		virtual void onAfrerDisappeared() {}

		// Called when window resized
		virtual void onWindowResized() {}
	};
}