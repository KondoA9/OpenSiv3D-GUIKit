#include <Aoba/Layer.hpp>

namespace s3d::aoba {
	const Vec2& Layer::center() const {
		return m_center;
	}

	const Constraint& Layer::top() const {
		return m_top;
	}

	const Constraint& Layer::bottom() const {
		return m_bottom;
	}

	const Constraint& Layer::centerY() const {
		return m_centerY;
	}

	const Constraint& Layer::height() const {
		return m_height;
	}

	const Constraint& Layer::left() const {
		return m_left;
	}

	const Constraint& Layer::right() const {
		return m_right;
	}

	const Constraint& Layer::centerX() const {
		return m_centerX;
	}

	const Constraint& Layer::width() const {
		return m_width;
	}

	void Layer::updateConstraints() {
		// y-axis constraints
		// top & bottom
		if (m_top.isExist() && m_bottom.isExist()) {
			m_top.updateConstraint();
			m_bottom.updateConstraint();
			m_height = m_bottom - m_top;
			m_centerY = m_top + m_height * 0.5;
		}
		// top / bottom & height
		else if (m_top.isExist() && m_height.isExist()) {
			m_top.updateConstraint();
			m_height.updateConstraint();
			m_bottom = m_top + m_height;
			m_centerY = m_top + m_height * 0.5;
		}
		else if (m_bottom.isExist() && m_height.isExist()) {
			m_bottom.updateConstraint();
			m_height.updateConstraint();
			m_top = m_bottom - m_height;
			m_centerY = m_top + m_height * 0.5;
		}
		// top / bottom / height & centerY
		else if (m_top.isExist() && m_centerY.isExist()) {
			m_top.updateConstraint();
			m_centerY.updateConstraint();
			m_height = (m_centerY - m_top) * 2;
			m_bottom = m_top + m_height;
		}
		else if (m_bottom.isExist() && m_centerY.isExist()) {
			m_bottom.updateConstraint();
			m_centerY.updateConstraint();
			m_height = (m_bottom - m_centerY) * 2;
			m_top = m_bottom - m_height;
		}
		else if (m_height.isExist() && m_centerY.isExist()) {
			m_height.updateConstraint();
			m_centerY.updateConstraint();
			m_top = m_centerY - m_height * 0.5;
			m_bottom = m_top + m_height;
		}
		else {
			Logger << U"Y-axis constraints are invalid ";
		}

		// x-axis constraints
		// left & right
		if (m_left.isExist() && m_right.isExist()) {
			m_left.updateConstraint();
			m_right.updateConstraint();
			m_width = m_right - m_left;
			m_centerX = m_left + m_width * 0.5;
		}
		// left / right & width
		else if (m_left.isExist() && m_width.isExist()) {
			m_left.updateConstraint();
			m_width.updateConstraint();
			m_right = m_left + m_width;
			m_centerX = m_left + m_width * 0.5;
		}
		else if (m_right.isExist() && m_width.isExist()) {
			m_right.updateConstraint();
			m_width.updateConstraint();
			m_left = m_right - m_width;
			m_centerX = m_left + m_width * 0.5;
		}
		// left / right / width & centerX
		else if (m_left.isExist() && m_centerX.isExist()) {
			m_left.updateConstraint();
			m_centerX.updateConstraint();
			m_width = (m_centerX - m_left) * 2;
			m_right = m_left + m_width;
		}
		else if (m_right.isExist() && m_centerX.isExist()) {
			m_right.updateConstraint();
			m_centerX.updateConstraint();
			m_width = (m_right - m_centerX) * 2;
			m_left = m_right - m_width;
		}
		else if (m_width.isExist() && m_centerX.isExist()) {
			m_width.updateConstraint();
			m_centerX.updateConstraint();
			m_left = m_centerX - m_width * 0.5;
			m_right = m_left + m_width;
		}
		else {
			Logger << U"X-axis constraints are invalid ";
		}

		m_center = Vec2(m_centerX, m_centerY);
	}

	void Layer::removeAllConstraints() {
		m_top.removeConstraint();
		m_bottom.removeConstraint();
		m_centerY.removeConstraint();
		m_height.removeConstraint();
		m_left.removeConstraint();
		m_right.removeConstraint();
		m_centerX.removeConstraint();
		m_width.removeConstraint();
	}
}
