#include "Test.h"

#include <Aoba/Aoba.hpp>

#include "PagingTest.h"
#include "ComponentsTest.h"
#include "FocusTest.h"

void AobaMain() {
	Window::Resize(1280, 720);

	aoba::AppendPage<LifeCycleTest>(U"LifeCycleTest");
	aoba::AppendPage<ComponentsTest>(U"ComponentsTest");
	aoba::AppendPage<FocusTest>(U"FocusTest");

	aoba::Start();

	pagingTest.validate();
}
