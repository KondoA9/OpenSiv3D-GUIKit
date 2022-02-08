#include "Aoba/PixelUnit.hpp"

#include <Siv3D.hpp>

int operator"" _px(unsigned long long int value) {
    return static_cast<int>(value * s3d::Window::GetState().scaling);
}

double operator"" _px(long double value) {
    return static_cast<double>(value * s3d::Window::GetState().scaling);
}
