#pragma once

#include "../Constraint/Constraint.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	enum class LayerDirection {
		Top,
		Bottom,
		CenterY,
		Height,
		Left,
		Right,
		CenterX,
		Width
	};

	struct Layer {
		Constraint top, bottom, centerY, height, left, right, centerX, width;

		void updateConstraints() {
			// y-axis constraints
			// top & bottom
			if (top.isConstraintExists() && bottom.isConstraintExists()) {
				top.updateConstraint();
				bottom.updateConstraint();
				height.value = bottom.value - top.value;
				centerY.value = top.value + height.value * 0.5;
			}
			// top / bottom & height
			else if (top.isConstraintExists() && height.isConstraintExists()) {
				top.updateConstraint();
				height.updateConstraint();
				bottom.value = top.value + height.value;
				centerY.value = top.value + height.value * 0.5;
			}
			else if (bottom.isConstraintExists() && height.isConstraintExists()) {
				bottom.updateConstraint();
				height.updateConstraint();
				top.value = bottom.value - height.value;
				centerY.value = top.value + height.value * 0.5;
			}
			// top / bottom / height & centerY
			else if (top.isConstraintExists() && centerY.isConstraintExists()) {
				top.updateConstraint();
				centerY.updateConstraint();
				height.value = (centerY.value - top.value) * 2;
				bottom.value = top.value + height.value;
			}
			else if (bottom.isConstraintExists() && centerY.isConstraintExists()) {
				bottom.updateConstraint();
				centerY.updateConstraint();
				height.value = (bottom.value - centerY.value) * 2;
				top.value = bottom.value - height.value;
			}
			else if (height.isConstraintExists() && centerY.isConstraintExists()) {
				height.updateConstraint();
				centerY.updateConstraint();
				top.value = centerY.value - height.value * 0.5;
				bottom.value = top.value + height.value;
			}
			else {
			}

			// x-axis constraints
			// left & right
			if (left.isConstraintExists() && right.isConstraintExists()) {
				left.updateConstraint();
				right.updateConstraint();
				width.value = right.value - left.value;
				centerX.value = left.value + width.value * 0.5;
			}
			// left / right & width
			else if (left.isConstraintExists() && width.isConstraintExists()) {
				left.updateConstraint();
				width.updateConstraint();
				right.value = left.value + width.value;
				centerX.value = left.value + width.value * 0.5;
			}
			else if (right.isConstraintExists() && width.isConstraintExists()) {
				right.updateConstraint();
				width.updateConstraint();
				left.value = right.value - width.value;
				centerX.value = left.value + width.value * 0.5;
			}
			// left / right / width & centerX
			else if (left.isConstraintExists() && centerX.isConstraintExists()) {
				left.updateConstraint();
				centerX.updateConstraint();
				width.value = (centerX.value - left.value) * 2;
				right.value = left.value + width.value;
			}
			else if (right.isConstraintExists() && centerX.isConstraintExists()) {
				right.updateConstraint();
				centerX.updateConstraint();
				width.value = (right.value - centerX.value) * 2;
				left.value = right.value - width.value;
			}
			else if (width.isConstraintExists() && centerX.isConstraintExists()) {
				width.updateConstraint();
				centerX.updateConstraint();
				left.value = centerX.value - width.value * 0.5;
				right.value = left.value + width.value;
			}
			else {
			}
		}

		Constraint* constraintPtr(LayerDirection direction) {
			return direction == LayerDirection::Top ? &top
				: direction == LayerDirection::Bottom ? &bottom
				: direction == LayerDirection::CenterY ? &centerY
				: direction == LayerDirection::Height ? &height
				: direction == LayerDirection::Left ? &left
				: direction == LayerDirection::Right ? &right
				: direction == LayerDirection::CenterX ? &centerX
				: direction == LayerDirection::Width ? &width : nullptr;
		}
	};
}