cmake_minimum_required (VERSION 3.0)

find_package(PkgConfig)

project(OpenSiv3D_GUIKit_Linux_Test)
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
	"../../Test/Example.cpp"
)

add_executable(GUIKit_Example ${SOURCE_FILES})

target_link_libraries(GUIKit_Example
	../OpenSiv3D/Linux/build/libSiv3D.a
	../../build/libSiv3DGUIKit.a
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