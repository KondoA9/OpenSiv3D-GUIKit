#include "GUIKitCore.hpp"

namespace s3d::gui::GUIKit {
	void Start();

	void SwitchPage(const String& identifier);

	void SetColorMode(ColorMode mode);

	void ToggleColorMode();

	void Terminate();

	// If you call this, you should call ContinueTermination() to terminate app
	void PreventTermination();

	void ContinueTermination();

	bool IsTerminationPrevented();

	/// <summary>
	/// Request to run a process on main thread. In many cases, func is the process that changes user interfaces.
	/// </summary>
	/// <param name="func">The process that runs on main thread.</param>
	void InsertProcessToMainThread(const std::function<void()>& func);

	/// <summary>
	/// Request to run a task parallelly, and if need, a completion process will runs on main thread.
	/// 
	/// </summary>
	/// <param name="func">The process that runs parallelly. Do not set a process that changes user interfaces.</param>
	/// <param name="completion">The process that runs on main thread after func() ended.</param>
	void CreateParallelTask(const std::function<void()>& func, const std::function<void()>& completion = std::function<void()>());

	bool IsParallelTaskAlive();

	/// <summary>
	/// Set an event with timeout. Do not set a process that changes user interfaces.
	/// </summary>
	/// <param name="func">A function that runs when timed out.</param>
	/// <param name="ms">The time to time out.</param>
	/// <param name="threading">If true, the function runs asynchronously.</param>
	/// <returns>The ID of the Timeout.</returns>
	size_t SetTimeout(const std::function<void()>& func, double ms, bool threading);

	bool StopTimeout(size_t id);

	bool RestartTimeout(size_t id);

	bool IsTimeoutAlive(size_t id);

	void AddDrawingEvent(const std::function<void()>& func);

	template<class T>
	T& GetPage(const String& identifier) noexcept {
		return GUIKitCore::Instance().getPage<T>(identifier);
	}

	template<class T>
	void AppendPage(const String& identifier) {
		return GUIKitCore::Instance().appendPage<T>(identifier);
	}
}
