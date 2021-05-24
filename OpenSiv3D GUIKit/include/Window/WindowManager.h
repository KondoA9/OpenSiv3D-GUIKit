#pragma once

#include <Siv3D.hpp>

namespace s3d::gui::WindowManager {
	void initialize();

	void update();

	bool didResized();
}

/*#include <ShellScalingApi.h>

class WindowManager {
	const Size baseWindowSize = Size(1280, 720);
	Size currentwindowSize = Size(0, 0);
	bool resized = false;

public:
	void initialize() {
		SetProcessDPIAware();
		Window::Resize(baseWindowSize, WindowResizeOption::ResizeSceneSize);
		Window::SetStyle(WindowStyle::Sizable);
	}

	void update() {
		const auto size = Window::ClientSize();
		resized = currentwindowSize != size;
		currentwindowSize = size;
	}

	bool didResized() {
		return resized;
	}
};*/