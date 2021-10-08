#include <Aoba/AobaInterface.hpp>

namespace s3d::aoba {
	void Start() {
		AobaCore::Instance().start();
	}

	void SwitchPage(const String& identifier) {
		AobaCore::Instance().switchPage(identifier);
	}

	void SetColorMode(ColorMode mode) {
		AobaCore::Instance().setColorMode(mode);
	}

	void ToggleColorMode() {
		AobaCore::Instance().toggleColorMode();
	}

	void Terminate() {
		AobaCore::Instance().terminate();
	}

	void PreventTermination() {
		AobaCore::Instance().preventTermination();
	}

	void ContinueTermination() {
		AobaCore::Instance().continueTermination();
	}

	bool IsTerminationPrevented() {
		return AobaCore::Instance().isTerminationPrevented();
	}

	void InsertProcessToMainThread(const std::function<void()>& func) {
		AobaCore::Instance().insertProcessToMainThread(func);
	}

	void CreateParallelTask(const std::function<void()>& func, const std::function<void()>& completion) {
		AobaCore::Instance().createParallelTask(func, completion);
	}

	bool IsParallelTaskAlive() {
		return AobaCore::Instance().isParalellTaskAlive();
	}

	size_t SetTimeout(const std::function<void()>& func, double ms, bool threading) {
		return AobaCore::Instance().setTimeout(func, ms, threading);
	}

	bool StopTimeout(size_t id) {
		return AobaCore::Instance().stopTimeout(id);
	}

	bool RestartTimeout(size_t id) {
		return AobaCore::Instance().restartTimeout(id);
	}

	bool IsTimeoutAlive(size_t id) {
		return AobaCore::Instance().isTimeoutAlive(id);
	}

	void AddDrawingEvent(const std::function<void()>& func) {
		return AobaCore::Instance().addDrawingEvent(func);
	}
}
