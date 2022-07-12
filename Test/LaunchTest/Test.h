#include <iostream>

namespace Test {
	void Terminate(std::string_view msg) {
		std::cerr << msg << std::endl;
		std::terminate();
	}

	void Assert(bool expression, std::string_view msg) {
		if (!expression) {
			Terminate(msg);
		}
	}

	std::string LogMessageTemplate() {
		return "[Log]: ";
	}

	void LogWithMsg(std::string_view msg) {
		std::cout << LogMessageTemplate() << msg << std::endl;
	}

	template<class T>
	void LogWithMsg(T* arg, std::string_view msg) {
		std::cout << LogMessageTemplate() << typeid(*arg).name() << ": " << msg << std::endl;
	}
}
