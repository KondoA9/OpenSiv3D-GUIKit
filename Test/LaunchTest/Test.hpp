#pragma once

#include <iostream>

namespace Test {
    void Terminate(std::string_view msg);

    void Assert(bool expression, std::string_view msg) ;

    std::string LogMessageTemplate() ;

    void LogWithMsg(std::string_view msg);

    template <class T>
    void LogWithMsg(T* arg, std::string_view msg) {
        std::cout << LogMessageTemplate() << typeid(*arg).name() << ": " << msg << std::endl;
    }
}
