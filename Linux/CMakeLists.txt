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
	"../Aoba/src/Core/Core.cpp"
	"../Aoba/src/Core/CoreLifeCycle.cpp"
	"../Aoba/src/Core/PageCycle.cpp"
	"../Aoba/src/Core/PageManager.cpp"
	"../Aoba/src/Core/TaskRunner.cpp"
	"../Aoba/src/Core/WindowManager.cpp"

	"../Aoba/src/AobaLog/AobaLog.cpp"

	"../Aoba/src/Factory/Factory.cpp"
	
	"../Aoba/src/ComponentStorage/ComponentStorage.cpp"

	"../Aoba/src/Page/Page.cpp"

	"../Aoba/src/ColorTheme/ColorTheme.cpp"

	"../Aoba/src/Imaging/Imaging.cpp"

	"../Aoba/src/PixelUnit/PixelUnit.cpp"

	"../Aoba/src/Timeout/Timeout.cpp"

	"../Aoba/src/CommandLine/CommandLine.cpp"

	"../Aoba/src/UserInterface/Font/UnifiedFont.cpp"
	"../Aoba/src/UserInterface/Constraint/Constraint.cpp"
	"../Aoba/src/UserInterface/Layer/Layer.cpp"

	"../Aoba/src/UserInterface/Component/UIComponent/UIComponent.cpp"
	"../Aoba/src/UserInterface/Component/UIComponent/UIComponentMouseEvent.cpp"
	"../Aoba/src/UserInterface/Component/UIButton/UIButton.cpp"
	"../Aoba/src/UserInterface/Component/UICircle/UICircle.cpp"
	"../Aoba/src/UserInterface/Component/UICheckBox/UICheckBox.cpp"
	"../Aoba/src/UserInterface/Component/UIIcon/UIIcon.cpp"
	"../Aoba/src/UserInterface/Component/UIImageView/UIImageView.cpp"
	"../Aoba/src/UserInterface/Component/UIInputField/UIInputField.cpp"
	"../Aoba/src/UserInterface/Component/UIInputField/KeyboardOperation/KeyboardOperation.cpp"
	"../Aoba/src/UserInterface/Component/UIRect/UIRect.cpp"
	"../Aoba/src/UserInterface/Component/UISlider/UISlider.cpp"
	"../Aoba/src/UserInterface/Component/UIText/UIText.cpp"
	"../Aoba/src/UserInterface/Component/UIToggleButton/UIToggleButton.cpp"
	"../Aoba/src/UserInterface/Component/UIView/UIView.cpp"
	"../Aoba/src/UserInterface/Component/UITabView/UITabView.cpp"
	"../Aoba/src/UserInterface/Component/UIVStackView/UIVStackView.cpp"
)