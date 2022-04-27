#include "Test.h"

#include "Aoba/Aoba.hpp"

#include "PagingTest.h"
#include "ComponentsTest.h"
#include "FocusTest.h"

void AobaMain() {
	Window::Resize(1280, 720);

	aoba::Core::AppendPage<LifeCycleTest>(U"LifeCycleTest");
	aoba::Core::AppendPage<ComponentsTest>(U"ComponentsTest");
	aoba::Core::AppendPage<FocusTest>(U"FocusTest");

	aoba::Core::Start();

	pagingTest.validate();
}
