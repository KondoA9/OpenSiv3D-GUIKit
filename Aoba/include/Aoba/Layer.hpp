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

	struct LayerSpace {
		double top = 0;
		double bottom = 0;
		double left = 0;
		double right = 0;
	};

	struct Layer {
	private:
		Constraint m_top, m_bottom, m_centerY, m_height, m_left, m_right, m_centerX, m_width;

		LayerSpace m_margin, m_padding;

		Vec2 m_center = Vec2();

	public:
		const Constraint& top() const;

		const Constraint& bottom() const;

		const Constraint& centerY() const;

		const Constraint& height() const;

		const Constraint& left() const;

		const Constraint& right() const;

		const Constraint& centerX() const;

		const Constraint& width() const;

		const Vec2& center() const;

		const LayerSpace& margin() const;

		const LayerSpace& padding() const;

		void updateConstraints();

		void setConstraint(LayerDirection direction, double constant, double multiplier);

		void setConstraint(LayerDirection direction, const std::function<double()>& func, double constant, double multiplier);

		void setConstraint(LayerDirection direction, Layer& otherLayer, LayerDirection otherLayerDirection, double constant, double multiplier);

		void removeConstraint(LayerDirection direction);

		void removeAllConstraints();

		void setMargin(const LayerSpace& layerSpace);

		void setPadding(const LayerSpace& layerSpace);

	private:
		Constraint& constraintReferenceTo(LayerDirection direction);
	};
}
