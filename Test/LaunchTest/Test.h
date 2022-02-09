#include <iostream>

namespace Test {
    void Terminate(const std::string& msg) {
        std::cerr << msg << std::endl;
        std::terminate();
    }

    void Assert(bool expression, const std::string& msg) {
        if (!expression) {
            Terminate(msg);
        }
    }

    std::string LogMessageTemplate() {
        return "[Log]: ";
    }

    void LogWithMsg(const std::string& msg) {
        std::cout << LogMessageTemplate() << msg << std::endl;
    }

    template <class T>
    void LogWithMsg(T* arg, const std::string& msg) {
        std::cout << LogMessageTemplate() << typeid(*arg).name() << ": " << msg << std::endl;
    }
}
