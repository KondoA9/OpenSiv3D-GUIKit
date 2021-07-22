cmake_minimum_required (VERSION 3.0)

find_package(PkgConfig)

project(GUIKit-LaunchTest)
enable_language(C)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -fPIC -msse4.1")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options ("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options ("-fdiagnostics-color=always")
endif()

pkg_check_modules(OPENCV4 REQUIRED opencv4)

include_directories(
	"/usr/include"

	"../../GUIKit/include"
	"../../Dependencies/include"
	"../../Dependencies/include/ThirdParty"
)

set(SOURCE_FILES
	"../../Test/LaunchTest/LaunchTest.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../App)

add_executable(GUIKit-LaunchTest ${SOURCE_FILES})

target_link_libraries(GUIKit-LaunchTest
	${PROJECT_SOURCE_DIR}/../../Dependencies/lib/Linux/libSiv3D.a
	${PROJECT_SOURCE_DIR}/../build/libSiv3DGUIKit.a
	${OPENCV4_LIBRARIES}
	-lOpenGL
	-lGLEW
	-lX11
	-lpthread
	-ldl
	-ludev
	-lfreetype
	-lharfbuzz
	-lglib-2.0
	-lgobject-2.0
	-lgio-2.0
	-lpng
	-lturbojpeg
	-lgif
	-lwebp
	-lopenal
	-logg
	-lvorbis
	-lvorbisenc
	-lvorbisfile
	-lboost_filesystem
	-lavformat
	-lavcodec
	-lavutil
	-lswresample
	-lz
)