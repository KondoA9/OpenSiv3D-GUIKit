#include "GUITest.hpp"

#include <Aoba/Aoba.hpp>

#include "Page/ComponentsTestPage.hpp"
#include "Page/FocusTestPage.hpp"
#include "Page/LifeCycleTestPage.hpp"

namespace GUITest {
    void Test() {
        aoba::Core::AppendPage<LifeCycleTestPage>(U"LifeCycleTest");
        aoba::Core::AppendPage<ComponentsTestPage>(U"ComponentsTest");
        aoba::Core::AppendPage<FocusTestPage>(U"FocusTest");

        aoba::Core::Start();

        aoba::Core::GetPage<LifeCycleTestPage>(U"LifeCycleTest").validate();
    }
}
