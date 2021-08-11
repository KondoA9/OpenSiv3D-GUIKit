#pragma once

#include <Siv3D.hpp>

namespace s3d::gui::WindowManager {
	void Initialize();

	void Update();

	bool DidResized();
}