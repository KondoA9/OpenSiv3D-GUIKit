#include "WindowManager.h"

#include <ShellScalingApi.h>

namespace s3d::gui::WindowManager {
	const Size baseWindowSize = Size(1280, 720);
	Size currentwindowSize = Size(0, 0);
	bool resized = false;

	void Initialize() {
		SetProcessDPIAware();
		Window::Resize(baseWindowSize, WindowResizeOption::ResizeSceneSize);
		Window::SetStyle(WindowStyle::Sizable);
	}

	void Update() {
		const auto size = Window::ClientSize();
		resized = currentwindowSize != size;
		currentwindowSize = size;
	}

	bool DidResized() {
		return resized;
	}
}