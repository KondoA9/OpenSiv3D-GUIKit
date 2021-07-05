#pragma once

#include "Timeout.h"
#include "Page.h"

#include <Siv3D.hpp>

#include <mutex>

namespace s3d::gui {
	enum class ColorMode;

	class GUIKit final {
	public:
		static GUIKit guikit;

	private:
		std::mutex m_mutex;

		Array<std::shared_ptr<Page>> m_pages;
		std::shared_ptr<Page> m_drawingPage, m_forwardPage, m_backwardPage;

		bool m_animateColor = false;
		bool m_pageChanging = false, m_preparePageChanging = false;

		Array<std::function<void()>> m_drawingEvents, m_eventsRequestedToRunInMainThread;
		Array<Timeout> m_timeouts;

	public:
		GUIKit(const GUIKit&) = delete;

		GUIKit(GUIKit&&) = delete;

		static GUIKit& Instance() {
			static GUIKit instance;
			return instance;
		}

		void start();

		void switchPage(const String& identifier);

		void setColorMode(ColorMode mode);

		void toggleColorMode();

		void insertToMainThread(const std::function<void()>& func);

		void insertAsyncProcess(const std::function<void()>& asyncFunc, const std::function<void()>& mainThreadFunc = std::function<void()>());

		size_t setTimeout(const std::function<void()>& func, double ms, bool threading);

		bool stopTimeout(size_t id);

		bool restartTimeout(size_t id);

		bool isTimeoutAlive(size_t id);

		void addDrawingEvent(const std::function<void()>& func) {
			m_drawingEvents.push_back(func);
		}

		template<class T>
		T* getPage(const String& identifier) {
			return getPagePtr<T>(identifier).get();
		}

		template<class T>
		void appendPage(const T& page) {
			m_pages.push_back(std::make_shared<T>(page));
		}

		GUIKit& operator=(const GUIKit&) = delete;

		GUIKit& operator=(GUIKit&&) = delete;

	private:
		GUIKit() {
			initialize();
		}

		~GUIKit() {}

		void initialize();

		void run();

		void update();

		// Return true until the start up page appeared
		bool updateOnStartUp();

		// Return true until the page changed
		bool updateOnPageChanging();

		void updateOnStable();

		void preparePageChanging();

		void termination();

		bool animateColor();

		template<class T>
		std::shared_ptr<T> getPagePtr(const String& identifier) const {
			for (const auto& page : m_pages) {
				if (page->m_identifier == identifier) {
					return std::dynamic_pointer_cast<T>(page);
				}
			}

			Logger << U"Error(GUIKitCore): A page identified as {} does not exist."_fmt(identifier);
			return nullptr;
		}
	};
}
