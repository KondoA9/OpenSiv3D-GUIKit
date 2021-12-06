#pragma once

#include "Timeout.hpp"
#include "Page.hpp"

#include <Siv3D.hpp>

#include <mutex>
#include <atomic>

namespace s3d::aoba {
	class UIComponent;
	enum class ColorMode;

	class AobaCore final {
	private:
		class PageManager* m_pageManager;
		class ParallelTaskManager* m_parallelTaskManager;

		std::mutex m_mainThreadInserterMutex;

		std::atomic<bool> m_terminationPrevented = false;

		bool m_animateColor = false;

		Array<std::function<void()>> m_drawingEvents, m_eventsRequestedToRunInMainThread;
		Array<Timeout> m_timeouts;

	public:
		AobaCore(const AobaCore&) = delete;

		AobaCore(AobaCore&&) = delete;

		AobaCore& operator=(const AobaCore&) = delete;

		AobaCore& operator=(AobaCore&&) = delete;

		static AobaCore& Instance() {
			static AobaCore instance;
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

		// If you call this, you should call ContinueTermination() to terminate app
		void preventTermination() {
			m_terminationPrevented = true;
		}

		void continueTermination() {
			m_terminationPrevented = false;
		}

		/// <summary>
		/// Request to run a process on main thread. In many cases, func is the process that changes user interfaces.
		/// </summary>
		/// <param name="func">The process that runs on main thread.</param>
		void insertProcessToMainThread(const std::function<void()>& func);

		/// <summary>
		/// Request to run a task parallelly, and if need, a completion process will runs on main thread.
		/// 
		/// </summary>
		/// <param name="func">The process that runs parallelly. Do not set a process that changes user interfaces.</param>
		/// <param name="completion">The process that runs on main thread after func() ended.</param>
		void createParallelTask(const std::function<void()>& func, const std::function<void()>& completion = std::function<void()>());

		/// <summary>
		/// Set an event with timeout. Do not set a process that changes user interfaces.
		/// </summary>
		/// <param name="func">A function that runs when timed out.</param>
		/// <param name="ms">The time to time out.</param>
		/// <param name="threading">If true, the function runs asynchronously.</param>
		/// <returns>The ID of the Timeout. ID is 1, 2, 3, ...</returns>
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

		void appendIsolatedComponent(const UIComponent& component);

	private:
		AobaCore();

		~AobaCore();

		static void AddLicense();

		Page& getPage(const String& identifier) const noexcept;

		void appendPage(const std::shared_ptr<Page>& page);

		void appendIsolatedComponent(const std::shared_ptr<UIComponent>& component);

		void run();

		void updateAoba();

		void updateMainThreadEvents();

		void updateTimeouts();

		bool animateColor();
	};
}
