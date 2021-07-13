#pragma once

#include "Timeout.h"
#include "Page.h"
#include "UIComponent.h"

#include <Siv3D.hpp>

#include <mutex>

namespace s3d::gui {
	enum class ColorMode;

	class GUIKit final {
		enum class PageTransition {
			StartUp,
			Stable,
			StartChanging,
			Changing,
			JustChanged
		};

	public:
		static GUIKit guikit;

	private:
		std::mutex m_mainThreadInserterMutex;

		Array<std::shared_ptr<Page>> m_pages;
		std::shared_ptr<Page> m_drawingPage, m_forwardPage, m_backwardPage;

		Array<std::shared_ptr<UIComponent>> m_isolatedComponents;

		PageTransition m_pageTransition = PageTransition::StartUp;

		bool m_animateColor = false;
		double m_pageTransitionRate = 1.0;
		Rect m_windowScissorRect;

		Array<std::function<void()>> m_drawingEvents, m_eventsRequestedToRunInMainThread;
		Array<Timeout> m_timeouts;

	public:
		GUIKit(const GUIKit&) = delete;

		GUIKit(GUIKit&&) = delete;

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

		static GUIKit& Instance() {
			static GUIKit instance;
			return instance;
		}

		void addDrawingEvent(const std::function<void()>& func) {
			m_drawingEvents.push_back(func);
		}

		template<class T>
		T& getPage(const String& identifier) const noexcept {
			return *getPagePtr<T>(identifier).get();
		}

		template<class T>
		void appendPage(const String& identifier) {
			m_pages.push_back(std::shared_ptr<T>(new T(identifier)));
		}

		template<class T>
		void appendIsolatedComponent(const std::shared_ptr<T>& component) {
			m_isolatedComponents.emplace_back(component);
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

		void updateGUIKit();

		// Return true until the start up page appeared
		bool updateOnStartUp();

		// Return true until the page changed
		bool updateOnPageChanging();

		void updateOnStable();

		void preparePageChanging();

		void finalizePageChanging();

		void update();

		void draw();

		void termination();

		void updateInputEventsStable();

		void updateLayerStable();

		void updateMainThreadEventsStable();

		void updateTimeoutsStable();

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
