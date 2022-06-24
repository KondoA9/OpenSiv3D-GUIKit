#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba::Algorithm {
    // Find the element from sorted vector by binary search and return its index.
    template <class T>
    Optional<size_t> FindElement(const Array<T>& vector, size_t key, const std::function<size_t(const T&)>& getValue) {
        if (vector.isEmpty()) {
            return none;
        }

        size_t left = 0, right = vector.size() - 1;

        while (left <= right) {
            const size_t mid = (left + right) / 2;
            const auto value = getValue(vector[mid]);

            if (value < key) {
                left = mid + 1;
            } else if (value > key) {
                right = mid - 1;
            } else {
                return mid;
            }
        }

        return none;
    }

    // Find the lower element of the element from sorted vector by binary search and return lower element's index.
    template <class T>
    size_t FindLowerElement(const Array<T>& vector, size_t key, const std::function<size_t(const T&)>& getValue) {
        if (vector.isEmpty()) {
            return 0;
        }

        size_t left = 0, right = vector.size() - 1;

        while (left <= right) {
            const size_t mid = (left + right) / 2;
            const auto value = getValue(vector[mid]);

            if (value < key) {
                left = mid + 1;
            } else if (value > key) {
                right = mid - 1;
            } else {
                return mid;
            }
        }

        return left;
    }
}
