cmake_minimum_required(VERSION 3.0)

project(OpenSiv3D_GUIKit_Linux)
enable_language(C)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1 -D_GLFW_X11")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1 -D_GLFW_X11")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options("-fdiagnostics-color=always")
endif()

include_directories(
	"../GUIKit/include"
	"../Dependencies/include"
	"../Dependencies/include/ThirdParty"
)

set(SOURCE_FILES
"../GUIKit/src/GUIKitCore/GUIKitCore.cpp"
"../GUIKit/src/GUIKitCore/GUIKitLifeCycle.cpp"

"../GUIKit/src/ColorTheme/ColorTheme.cpp"

"../GUIKit/src/Imaging/Imaging.cpp"

"../GUIKit/src/PixelUnit/PixelUnit.cpp"

"../GUIKit/src/Timeout/Timeout.cpp"

"../GUIKit/src/Window/WindowManager.cpp"

"../GUIKit/src/UserInterface/Font/UnifiedFont.cpp"
"../GUIKit/src/UserInterface/Constraint/Constraint.cpp"
"../GUIKit/src/UserInterface/Layer/Layer.cpp"
"../GUIKit/src/UserInterface/Component/UIComponent.cpp"
"../GUIKit/src/UserInterface/Component/UIComponentMouseEvent.cpp"
"../GUIKit/src/UserInterface/Component/UIButton/UIButton.cpp"
"../GUIKit/src/UserInterface/Component/UICircle/UICircle.cpp"
"../GUIKit/src/UserInterface/Component/UIIcon/UIIcon.cpp"
"../GUIKit/src/UserInterface/Component/UIImageView/UIImageView.cpp"
"../GUIKit/src/UserInterface/Component/UIImageView/UIZStackedImageView.cpp"
"../GUIKit/src/UserInterface/Component/UIInputField/UIInputField.cpp"
"../GUIKit/src/UserInterface/Component/UIRect/UIRect.cpp"
"../GUIKit/src/UserInterface/Component/UISlider/UISlider.cpp"
"../GUIKit/src/UserInterface/Component/UIText/UIText.cpp"
"../GUIKit/src/UserInterface/Component/UIToggleButton/UIToggleButton.cpp"
"../GUIKit/src/UserInterface/Component/UIView/UIView.cpp"
"../GUIKit/src/UserInterface/Component/UIVStackView/UIVStackView.cpp"
)

add_library(Siv3DGUIKit STATIC ${SOURCE_FILES})

target_link_libraries(Siv3DGUIKit)