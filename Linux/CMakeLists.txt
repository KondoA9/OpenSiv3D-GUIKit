cmake_minimum_required(VERSION 3.1)

enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1 -D_GLFW_X11")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options("-fdiagnostics-color=always")
endif()

project(Aoba_Linux)

include_directories(
	SYSTEM "../Dependencies/include"
	SYSTEM "../Dependencies/include/ThirdParty"
	"../Aoba"
	"../Aoba/include"
)

add_library(aoba STATIC
	"../Aoba/src/AobaLog/AobaLog.cpp"

	"../Aoba/src/ColorTheme/ColorTheme.cpp"

	"../Aoba/src/CommandLine/CommandLine.cpp"

	"../Aoba/src/Component/UIButton.cpp"
	"../Aoba/src/Component/UICheckBox.cpp"
	"../Aoba/src/Component/UICircle.cpp"
	"../Aoba/src/Component/UIComponent.cpp"
	"../Aoba/src/Component/UIComponentMouseEvent.cpp"
	"../Aoba/src/Component/UIIcon.cpp"
	"../Aoba/src/Component/UIImageView.cpp"
	"../Aoba/src/Component/UIInputField.cpp"
	"../Aoba/src/Component/UIInputField/KeyboardOperation.cpp"
	"../Aoba/src/Component/UIRect.cpp"
	"../Aoba/src/Component/UISlider.cpp"
	"../Aoba/src/Component/UITabView.cpp"
	"../Aoba/src/Component/UIText.cpp"
	"../Aoba/src/Component/UIToggleButton.cpp"
	"../Aoba/src/Component/UIView.cpp"
	"../Aoba/src/Component/UIVStackView.cpp"
	
	"../Aoba/src/ComponentStorage/ComponentStorage.cpp"

	"../Aoba/src/Constraint/Constraint.cpp"

	"../Aoba/src/Core/Core.cpp"
	"../Aoba/src/Core/CoreLifeCycle.cpp"
	"../Aoba/src/Core/PageCycle.cpp"
	"../Aoba/src/Core/PageManager.cpp"
	"../Aoba/src/Core/TaskRunner.cpp"
	"../Aoba/src/Core/TaskRunner/AsyncTaskManager.cpp"
	"../Aoba/src/Core/TaskRunner/SyncTaskManager.cpp"
	"../Aoba/src/Core/TaskRunner/Timeout.cpp"
	"../Aoba/src/Core/TaskRunner/TimeoutTaskManager.cpp"
	"../Aoba/src/Core/WindowManager.cpp"

	"../Aoba/src/Factory/Factory.cpp"

	"../Aoba/src/Imaging/Imaging.cpp"

	"../Aoba/src/InputEvent/InputEvent.cpp"
	"../Aoba/src/InputEvent/InputEventManager.cpp"

	"../Aoba/src/Layer/Layer.cpp"

	"../Aoba/src/Page/Page.cpp"

	"../Aoba/src/PixelUnit/PixelUnit.cpp"

	"../Aoba/src/Tooltip/Tooltip.cpp"

	"../Aoba/src/UnifiedFont/UnifiedFont.cpp"
)