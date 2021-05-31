#include "WindowManager.h"

#if SIV3D_PLATFORM(WINDOWS)
#include <ShellScalingApi.h>
#endif

namespace s3d::gui::WindowManager {
	const Size baseWindowSize = Size(1280, 720);
	Size currentwindowSize = Size(0, 0);
	bool resized = false;

	void Initialize() {
#if SIV3D_PLATFORM(WINDOWS)
		SetProcessDPIAware();
#endif

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