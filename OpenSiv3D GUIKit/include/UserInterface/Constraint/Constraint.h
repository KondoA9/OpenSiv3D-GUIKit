#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	struct Constraint {
		double value;

	private:
		bool m_exists = false;
		std::function<double()> m_func;
		double* m_watchingValue = nullptr;
		double m_constant = 0.0;
		double m_multiplier = 1.0;

	public:
		~Constraint() {}

		bool isConstraintExists() const {
			return m_exists;
		}

		void setConstraint(double constant = 0.0, double multiplier = 1.0) {
			m_constant = constant;
			m_multiplier = multiplier;
			m_exists = true;
		}

		void setConstraint(double* watchingValue, double constant = 0.0, double multiplier = 1.0) {
			m_watchingValue = watchingValue;
			setConstraint(constant, multiplier);
		}

		void setConstraint(const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0) {
			m_func = func;
			setConstraint(constant, multiplier);
		}

		void removeConstraint() {
			m_exists = false;
			m_func = std::function<double()>();
			m_watchingValue = nullptr;
			m_constant = 0.0;
			m_multiplier = 1.0;
		}

		void updateConstraint() {
			value = m_func ? m_func() * m_multiplier + m_constant
				: m_watchingValue == nullptr ? m_constant : *m_watchingValue * m_multiplier + m_constant;
		}
	};
}