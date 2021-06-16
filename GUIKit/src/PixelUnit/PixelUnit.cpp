#include <GUIKit/PixelUnit.h>

#include <Siv3D.hpp>

int operator"" _px(unsigned long long int value) {
	return static_cast<int>(value * Graphics::GetDPIScaling());
}

double operator"" _px(long double value) {
	return static_cast<double>(value * Graphics::GetDPIScaling());
}