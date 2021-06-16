#pragma once

#include "Timeout.h"
#include "Page.h"

#include <Siv3D.hpp>

#include <thread>
#include <mutex>

namespace s3d::gui {
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
		Array<Timeout> m_timeouts;

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

		void insertToMainThread(const std::function<void()>& func);

		void insertAsyncProcess(const std::function<void()>& heavyFunc, const std::function<void()>& uiUpdatingFunc);

		size_t setTimeout(const std::function<void()>& func, double ms, bool threading);

		bool stopTimeout(size_t id);

		bool restartTimeout(size_t id);

		bool isTimeoutAlive(size_t id);

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

		void termination();

		void animateColor();
	};
}
