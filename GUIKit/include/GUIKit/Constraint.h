#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	struct Constraint {
	public:
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

		void updateConstraint() {
			value = m_func ? m_func() * m_multiplier + m_constant
				: m_watchingValue == nullptr ? m_constant : *m_watchingValue * m_multiplier + m_constant;
		}

		void setConstraint(double constant = 0.0, double multiplier = 1.0);

		void setConstraint(double* watchingValue, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0);

		void removeConstraint();

		/*operator double() const {
			return value;
		}*/
	};
}