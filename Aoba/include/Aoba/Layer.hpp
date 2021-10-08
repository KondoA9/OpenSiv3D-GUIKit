#pragma once

#include "Constraint.hpp"

#include <Siv3D.hpp>

namespace s3d::aoba {
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

		void updateConstraints();

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
