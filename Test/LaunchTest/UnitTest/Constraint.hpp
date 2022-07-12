#pragma once

#include <Aoba/Aoba.hpp>

#include "../Test.hpp"

namespace UnitTest::Constraint {
    namespace _internal {
        void TestByValue() {
            aoba::Constraint c;

            c.setConstraint(1.0);
            c.updateConstraint();
            Test::Assert(c == 1.0, "Could not set the constraint by value");
        }

        void TestByPointer() {
            double value = 1.0;
            aoba::Constraint c;

            c.setConstraint(&value);
            c.updateConstraint();
            Test::Assert(c == 1.0, "Could not set the constraint by pointer");

            value = 2.0;
            c.updateConstraint();
            Test::Assert(c == 2.0, "Could not set the constraint by pointer");

            // Pointer, constant and multiplier
            value = 3.0;
            c.setConstraint(&value, 1.0, 2.0);  // 3.0 * 2.0 + 1.0 = 7.0
            c.updateConstraint();
            Test::Assert(c == 7.0, "Could not set the constraint by pointer, constant and multiplier");
        }

        void TestByFunc() {
            const auto f = [] { return 3.0; };
            aoba::Constraint c;

            c.setConstraint(f);
            c.updateConstraint();
            Test::Assert(c == 3.0, "Could not set the constraint by function");

            // Func, constant and multiplier
            c.setConstraint(f, 1.0, 2.0);  // 3.0 * 2.0 + 1.0 = 7.0
            c.updateConstraint();
            Test::Assert(c == 7.0, "Could not set the constraint by function, constant and multiplier");
        }
    }

    void Test() {
        _internal::TestByValue();
        _internal::TestByPointer();
        _internal::TestByFunc();
    }
}
