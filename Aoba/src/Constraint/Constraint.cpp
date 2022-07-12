#include "Aoba/Constraint.hpp"

namespace s3d::aoba {
    void Constraint::setConstraint(double constant) noexcept {
        m_constant = constant;
        m_exists   = true;

        // Reset
        m_multiplier    = 1.0;
        m_watchingValue = nullptr;
        m_func          = nullptr;
    }

    void Constraint::setConstraint(double* const watchingValue, double constant, double multiplier) noexcept {
        m_constant      = constant;
        m_multiplier    = multiplier;
        m_watchingValue = watchingValue;
        m_exists        = true;

        // Reset
        m_func = nullptr;
    }

    void Constraint::setConstraint(const std::function<double()>& func, double constant, double multiplier) {
        m_constant   = constant;
        m_multiplier = multiplier;
        m_func       = func;
        m_exists     = true;

        // Reset
        m_watchingValue = nullptr;
    }

    void Constraint::removeConstraint() noexcept {
        m_constant      = 0.0;
        m_multiplier    = 1.0;
        m_watchingValue = nullptr;
        m_func          = nullptr;
        m_exists        = false;
    }
}
