#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	class Page;
	enum class ColorMode;

	class GUIKit {
		String m_title;

		Array<std::shared_ptr<Page>> m_pages;

		std::shared_ptr<Page> m_drawingPage;

		// When the page changed, forwardPage will be appeared and backwardPage will be disappeared.
		std::shared_ptr<Page> m_forwardPage, m_backwardPage;

		bool m_animateColor = false;
		bool m_uiChanging = false;

	public:
		GUIKit() {
			initialize();
		}

		~GUIKit() {}

		void start();

		void setTitle(const String& title);

		void switchPage(const String& identifier);

		void setColorMode(ColorMode mode);

		void toggleColorMode();

		template<class T>
		void appendPage(T page) {
			page.guikit = this;
			m_pages.push_back(std::make_shared<T>(page));
		}

		template<class T>
		std::shared_ptr<T> getUserInterface(const String& identifier) const {
			for (const auto& page : m_pages) {
				if (page->identifier == identifier) {
					return std::dynamic_pointer_cast<T>(page);
				}
			}

			Logger << U"Error(GUIKitCore): UserInterface identified as {} does not exist."_fmt(identifier);
			return nullptr;
		}

	private:
		void initialize();

		void run();

		void update();

		void animateColor();
	};
}