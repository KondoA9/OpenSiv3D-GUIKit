#pragma once

#include <Siv3D.hpp>

namespace s3d::gui::WindowManager {
	void initialize();

	void update();

	bool didResized();
}