#include "WindowManager.hpp"

#include <Siv3D.hpp>

namespace s3d::aoba::WindowManager {
    Size windowSize = Size(0, 0);
    bool didResized = false;

    void Initialize() {
        Window::SetStyle(WindowStyle::Sizable);
    }

    void Update() {
        const auto size = Window::GetState().frameBufferSize;
        didResized      = windowSize != size;
        windowSize      = size;
    }

    bool DidResized() {
        return didResized;
    }
}
