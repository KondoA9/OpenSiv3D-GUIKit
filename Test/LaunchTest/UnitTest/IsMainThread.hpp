#pragma once

#include <Aoba/Aoba.hpp>

#include "../Test.hpp"

namespace UnitTest::IsMainThread {
    void Test() {
        Test::Assert(aoba::Core::IsMainThread(), "This is in main thread but recognized as not in main.");

        auto thread = std::thread(
            [] { Test::Assert(!aoba::Core::IsMainThread(), "This is not in main thread but recognized as in main."); });

        thread.join();
    }
}
