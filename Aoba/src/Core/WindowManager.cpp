#include "WindowManager.hpp"

#include <Siv3D.hpp>

namespace s3d::aoba::WindowManager {
    Size currentwindowSize = Size(0, 0);
    bool resized           = false;

    void Initialize() {
        Window::SetStyle(WindowStyle::Sizable);
    }

    void Update() {
        const auto size   = Window::GetState().frameBufferSize;
        resized           = currentwindowSize != size;
        currentwindowSize = size;
    }

    bool DidResized() {
        return resized;
    }
}
