#pragma once

#include <Siv3D.hpp>

#include "Constraint.hpp"

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
	private:
		Constraint m_top, m_bottom, m_centerY, m_height, m_left, m_right, m_centerX, m_width;

		Vec2 m_center = Vec2();

	public:

		const Vec2& center() const;

		const Constraint& top() const;

		const Constraint& bottom() const;

		const Constraint& centerY() const;

		const Constraint& height() const;

		const Constraint& left() const;

		const Constraint& right() const;

		const Constraint& centerX() const;

		const Constraint& width() const;

		void updateConstraints();

		void removeAllConstraints();

		Constraint* constraintPtr(LayerDirection direction) {
			return direction == LayerDirection::Top ? &m_top
				: direction == LayerDirection::Bottom ? &m_bottom
				: direction == LayerDirection::CenterY ? &m_centerY
				: direction == LayerDirection::Height ? &m_height
				: direction == LayerDirection::Left ? &m_left
				: direction == LayerDirection::Right ? &m_right
				: direction == LayerDirection::CenterX ? &m_centerX
				: direction == LayerDirection::Width ? &m_width : nullptr;
		}
	};
}
