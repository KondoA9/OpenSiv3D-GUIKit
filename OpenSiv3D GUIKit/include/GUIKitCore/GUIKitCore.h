#pragma once

#include <Siv3D.hpp>

#include <mutex>

namespace s3d::gui {
	class Page;
	enum class ColorMode;

	class GUIKit {
		std::mutex m_mtx;

		String m_title;

		Array<std::shared_ptr<Page>> m_pages;
		std::shared_ptr<Page> m_drawingPage;
		// When the page changed, forwardPage will be appeared and backwardPage will be disappeared.
		std::shared_ptr<Page> m_forwardPage, m_backwardPage;

		bool m_animateColor = false;
		bool m_uiChanging = false;

		Array<std::function<void()>> m_drawingEvents, m_eventsRequestedToRunInMainThread;

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

		void addDrawingEvent(const std::function<void()>& func) {
			m_drawingEvents.push_back(func);
		}

		void insertToMainThread(const std::function<void()>& func) {
			std::lock_guard<std::mutex> lock(m_mtx);
			m_eventsRequestedToRunInMainThread.push_back(func);
		}

		template<class T>
		void appendPage(T page) {
			page.m_guikit = this;
			m_pages.push_back(std::make_shared<T>(page));
		}

		template<class T>
		std::shared_ptr<T> getUserInterface(const String& identifier) const {
			for (const auto& page : m_pages) {
				if (page->m_identifier == identifier) {
					return std::dynamic_pointer_cast<T>(page);
				}
			}

			Logger << U"Error(GUIKitCore): A page identified as {} does not exist."_fmt(identifier);
			return nullptr;
		}

	private:
		void initialize();

		void run();

		void update();

		void animateColor();
	};
}