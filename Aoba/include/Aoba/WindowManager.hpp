#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba::WindowManager {
	void Initialize();

	void Update();

	bool DidResized();
}