#include <Aoba/Aoba.hpp>

#include "GUITest/GUITest.hpp"
#include "UnitTest/UnitTest.hpp"

void AobaMain() {
    Window::Resize(1280, 720);

    UnitTest::Constraint::Test();
    UnitTest::IsMainThread::Test();

    GUITest::Test();
}
