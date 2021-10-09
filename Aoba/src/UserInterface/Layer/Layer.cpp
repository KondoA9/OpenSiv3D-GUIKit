#include <Aoba/Layer.hpp>

namespace s3d::aoba {
	void Layer::updateConstraints() {
		// y-axis constraints
		// top & bottom
		if (top.isExist() && bottom.isExist()) {
			top.updateConstraint();
			bottom.updateConstraint();
			height = bottom - top;
			centerY = top + height * 0.5;
		}
		// top / bottom & height
		else if (top.isExist() && height.isExist()) {
			top.updateConstraint();
			height.updateConstraint();
			bottom = top + height;
			centerY = top + height * 0.5;
		}
		else if (bottom.isExist() && height.isExist()) {
			bottom.updateConstraint();
			height.updateConstraint();
			top = bottom - height;
			centerY = top + height * 0.5;
		}
		// top / bottom / height & centerY
		else if (top.isExist() && centerY.isExist()) {
			top.updateConstraint();
			centerY.updateConstraint();
			height = (centerY - top) * 2;
			bottom = top + height;
		}
		else if (bottom.isExist() && centerY.isExist()) {
			bottom.updateConstraint();
			centerY.updateConstraint();
			height = (bottom - centerY) * 2;
			top = bottom - height;
		}
		else if (height.isExist() && centerY.isExist()) {
			height.updateConstraint();
			centerY.updateConstraint();
			top = centerY - height * 0.5;
			bottom = top + height;
		}
		else {
			Logger << U"Y-axis constraints are invalid ";
		}

		// x-axis constraints
		// left & right
		if (left.isExist() && right.isExist()) {
			left.updateConstraint();
			right.updateConstraint();
			width = right - left;
			centerX = left + width * 0.5;
		}
		// left / right & width
		else if (left.isExist() && width.isExist()) {
			left.updateConstraint();
			width.updateConstraint();
			right = left + width;
			centerX = left + width * 0.5;
		}
		else if (right.isExist() && width.isExist()) {
			right.updateConstraint();
			width.updateConstraint();
			left = right - width;
			centerX = left + width * 0.5;
		}
		// left / right / width & centerX
		else if (left.isExist() && centerX.isExist()) {
			left.updateConstraint();
			centerX.updateConstraint();
			width = (centerX - left) * 2;
			right = left + width;
		}
		else if (right.isExist() && centerX.isExist()) {
			right.updateConstraint();
			centerX.updateConstraint();
			width = (right - centerX) * 2;
			left = right - width;
		}
		else if (width.isExist() && centerX.isExist()) {
			width.updateConstraint();
			centerX.updateConstraint();
			left = centerX - width * 0.5;
			right = left + width;
		}
		else {
			Logger << U"X-axis constraints are invalid ";
		}

		m_center = Vec2(centerX, centerY);
	}
}
