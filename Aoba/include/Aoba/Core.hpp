#pragma once

#include "Timeout.hpp"
#include "Page.hpp"

#include <Siv3D.hpp>

#include <mutex>
#include <atomic>

namespace s3d::aoba {
	class UIComponent;
	enum class ColorMode;

	class Core final {
	private:
		class PageManager* m_pageManager;
		class ParallelTaskManager* m_parallelTaskManager;

		std::mutex m_mainThreadInserterMutex;

		std::atomic<bool> m_terminationPrevented = false;

		bool m_animateColor = false;

		Array<std::function<void()>> m_drawingEvents, m_eventsRequestedToRunInMainThread;
		Array<Timeout> m_timeouts;

	public:
		Core(const Core&) = delete;

		Core(Core&&) = delete;

		Core& operator=(const Core&) = delete;

		Core& operator=(Core&&) = delete;

		static Core& Instance() {
			static Core instance;
			return instance;
		}

		static bool IsTerminationPrevented() {
			return Instance().m_terminationPrevented;
		}

		static bool IsParallelTaskAlive();

		static void Start();

		static void SwitchPage(const String& identifier);

		static void SetColorMode(ColorMode mode);

		static void ToggleColorMode();

		static void Terminate();

		// If you call this, you should call ContinueTermination() to terminate app
		static void PreventTermination() {
			Instance().m_terminationPrevented = true;
		}

		static void ContinueTermination() {
			Instance().m_terminationPrevented = false;
		}

		/// <summary>
		/// Request to run a process on main thread. In many cases, func is the process that changes user interfaces.
		/// </summary>
		/// <param name="func">The process that runs on main thread.</param>
		static void InsertProcessToMainThread(const std::function<void()>& func);

		/// <summary>
		/// Request to run a task parallelly, and if need, a completion process will runs on main thread.
		/// 
		/// </summary>
		/// <param name="func">The process that runs parallelly. Do not set a process that changes user interfaces.</param>
		/// <param name="completion">The process that runs on main thread after func() ended.</param>
		static void CreateParallelTask(const std::function<void()>& func, const std::function<void()>& completion = std::function<void()>());

		/// <summary>
		/// Set an event with timeout. Do not set a process that changes user interfaces.
		/// </summary>
		/// <param name="func">A function that runs when timed out.</param>
		/// <param name="ms">The time to time out.</param>
		/// <param name="threading">If true, the function runs asynchronously.</param>
		/// <returns>The ID of the Timeout. ID is 1, 2, 3, ...</returns>
		static size_t SetTimeout(const std::function<void()>& func, double ms, bool threading);

		static bool StopTimeout(size_t id);

		static bool RestartTimeout(size_t id);

		static bool IsTimeoutAlive(size_t id);

		static void AddDrawingEvent(const std::function<void()>& func) {
			Instance().m_drawingEvents.push_back(func);
		}

		template<class T>
		static T& GetPage(const String& identifier) noexcept {
			return static_cast<T&>(Instance().getPage(identifier));
		}

		template<class T>
		static void AppendPage(const String& identifier) {
			Instance().appendPage(std::shared_ptr<T>(new T(identifier)));
		}

		static void AppendIsolatedComponent(const UIComponent& component);

	private:
		Core();

		~Core();

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
