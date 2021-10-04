#include <GUIKit/GUIKitInterface.hpp>

namespace s3d::gui::GUIKit {
	void Start() {
		GUIKitCore::Instance().start();
	}

	void SwitchPage(const String& identifier) {
		GUIKitCore::Instance().switchPage(identifier);
	}

	void SetColorMode(ColorMode mode) {
		GUIKitCore::Instance().setColorMode(mode);
	}

	void ToggleColorMode() {
		GUIKitCore::Instance().toggleColorMode();
	}

	void Terminate() {
		GUIKitCore::Instance().terminate();
	}

	void PreventTermination() {
		GUIKitCore::Instance().preventTermination();
	}

	void ContinueTermination() {
		GUIKitCore::Instance().continueTermination();
	}

	bool IsTerminationPrevented() {
		return GUIKitCore::Instance().isTerminationPrevented();
	}

	void InsertProcessToMainThread(const std::function<void()>& func) {
		GUIKitCore::Instance().insertProcessToMainThread(func);
	}

	void CreateParallelTask(const std::function<void()>& func, const std::function<void()>& completion) {
		GUIKitCore::Instance().createParallelTask(func, completion);
	}

	bool IsParallelTaskAlive() {
		return GUIKitCore::Instance().isParalellTaskAlive();
	}

	size_t SetTimeout(const std::function<void()>& func, double ms, bool threading) {
		return GUIKitCore::Instance().setTimeout(func, ms, threading);
	}

	bool StopTimeout(size_t id) {
		return GUIKitCore::Instance().stopTimeout(id);
	}

	bool RestartTimeout(size_t id) {
		return GUIKitCore::Instance().restartTimeout(id);
	}

	bool IsTimeoutAlive(size_t id) {
		return GUIKitCore::Instance().isTimeoutAlive(id);
	}

	void AddDrawingEvent(const std::function<void()>& func) {
		return GUIKitCore::Instance().addDrawingEvent(func);
	}
}
