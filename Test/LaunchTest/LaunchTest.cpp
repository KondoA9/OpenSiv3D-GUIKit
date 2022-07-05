#include "Aoba/Aoba.hpp"

// clang-format off
#include "Test.h"
// clang-format on

#include "ComponentsTest.h"
#include "FocusTest.h"
#include "PagingTest.h"

void AobaMain() {
    // Test threading
    Test::Assert(aoba::Core::IsMainThread(), "This is in main thread but recognized as not in main.");
    std::thread([] {
        Test::Assert(!aoba::Core::IsMainThread(), "This is not in main thread but recognized as in main.");
    }).join();

    Window::Resize(1280, 720);

    aoba::Core::AppendPage<LifeCycleTest>(U"LifeCycleTest");
    aoba::Core::AppendPage<ComponentsTest>(U"ComponentsTest");
    aoba::Core::AppendPage<FocusTest>(U"FocusTest");

    aoba::Core::Start();

    pagingTest.validate();
}
