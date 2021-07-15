#pragma once

#include <Siv3D.hpp>

namespace s3d::gui {
	struct Layer;
	class UIComponent;

	struct Constraint {
		friend Layer;
		friend UIComponent;

	private:
		double m_value = 0;
		bool m_exists = false;
		std::function<double()> m_func;
		double* m_watchingValue = nullptr;
		double m_constant = 0.0;
		double m_multiplier = 1.0;

	public:
		Constraint() = default;

		~Constraint() = default;

		bool isExist() const {
			return m_exists;
		}

		void updateConstraint() {
			m_value = m_func ? m_func() * m_multiplier + m_constant
				: m_watchingValue == nullptr ? m_constant : *m_watchingValue * m_multiplier + m_constant;
		}

		void setConstraint(double constant = 0.0, double multiplier = 1.0);

		void setConstraint(double* watchingValue, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0);

		void removeConstraint();

		operator double() const {
			return m_value;
		}

	private:
		// Return the pointer of the value
		double* data() {
			return &m_value;
		}

		Constraint& operator =(double value) {
			m_value = value;
			return *this;
		}
	};
}
