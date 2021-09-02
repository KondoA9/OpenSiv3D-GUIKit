#include <GUIKit/WindowManager.hpp>

namespace s3d::gui::WindowManager {
	Size currentwindowSize = Size(0, 0);
	bool resized = false;

	void Initialize() {
		Window::SetStyle(WindowStyle::Sizable);
	}

	void Update() {
		const auto size = Window::GetState().frameBufferSize;
		resized = currentwindowSize != size;
		currentwindowSize = size;
	}

	bool DidResized() {
		return resized;
	}
}
