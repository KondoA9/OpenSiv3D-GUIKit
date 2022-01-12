#include <Aoba/Constraint.hpp>

namespace s3d::aoba {
	void Constraint::setConstraint(double constant, double multiplier) {
		m_constant = constant;
		m_multiplier = multiplier;
		m_exists = true;
	}

	void Constraint::setConstraint(double* const watchingValue, double constant, double multiplier) {
		m_watchingValue = watchingValue;
		setConstraint(constant, multiplier);
	}

	void Constraint::setConstraint(const std::function<double()>& func, double constant, double multiplier) {
		m_func = func;
		setConstraint(constant, multiplier);
	}

	void Constraint::removeConstraint() {
		m_exists = false;
		m_func = std::function<double()>();
		m_watchingValue = nullptr;
		m_constant = 0.0;
		m_multiplier = 1.0;
	}
}
