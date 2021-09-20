#pragma once

#include "Timeout.hpp"
#include "Page.hpp"
#include "GUIFactory.hpp"

#include <Siv3D.hpp>

#include <mutex>
#include <atomic>

namespace s3d::gui {
	enum class ColorMode;

	class GUIKitCore final {
	private:
		class PageManager* m_pageManager;
		class ParallelTaskManager* m_parallelTaskManager;

		std::mutex m_mainThreadInserterMutex;

		std::atomic<bool> m_terminationPrevented = false;

		bool m_animateColor = false;

		Array<std::function<void()>> m_drawingEvents, m_eventsRequestedToRunInMainThread;
		Array<Timeout> m_timeouts;

	public:
		GUIKitCore(const GUIKitCore&) = delete;

		GUIKitCore(GUIKitCore&&) = delete;

		GUIKitCore& operator=(const GUIKitCore&) = delete;

		GUIKitCore& operator=(GUIKitCore&&) = delete;

		static GUIKitCore& Instance() {
			static GUIKitCore instance;
			return instance;
		}

		bool isTerminationPrevented() const {
			return m_terminationPrevented;
		}

		bool isParalellTaskAlive() const;

		void start();

		void switchPage(const String& identifier);

		void setColorMode(ColorMode mode);

		void toggleColorMode();

		void terminate();

		void preventTermination() {
			m_terminationPrevented = true;
		}

		void continueTermination() {
			m_terminationPrevented = false;
		}

		void insertProcessToMainThread(const std::function<void()>& func);

		void createParallelTask(const std::function<void()>& func, const std::function<void()>& completion = std::function<void()>());

		size_t setTimeout(const std::function<void()>& func, double ms, bool threading);

		bool stopTimeout(size_t id);

		bool restartTimeout(size_t id);

		bool isTimeoutAlive(size_t id);

		void addDrawingEvent(const std::function<void()>& func) {
			m_drawingEvents.push_back(func);
		}

		template<class T>
		T& getPage(const String& identifier) const noexcept {
			return static_cast<T&>(getPage(identifier));
		}

		template<class T>
		void appendPage(const String& identifier) {
			appendPage(std::shared_ptr<T>(new T(identifier)));
		}

		template<class T>
		void appendIsolatedComponent(const T& component) {
			appendIsolatedComponent(GUIFactory::GetComponent(component.id()));
		}

	private:
		GUIKitCore();

		~GUIKitCore() = default;

		static void AddLicence();

		Page& getPage(const String& identifier) const noexcept;

		void appendPage(const std::shared_ptr<Page>& page);

		void appendIsolatedComponent(const std::shared_ptr<UIComponent>& component);

		void run();

		void updateGUIKit();

		void updateMainThreadEvents();

		void updateTimeouts();

		bool animateColor();
	};
}
