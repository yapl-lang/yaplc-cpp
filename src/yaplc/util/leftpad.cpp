#include "leftpad.h"

namespace yaplc { namespace util {
	std::string leftpad(const std::string &string, unsigned long count, char symbol) {
		return std::string(count, symbol) + string;
	}
	
	std::ostream &leftpad(std::ostream &stream, unsigned long count, char symbol) {
		for (; count != 0; --count) {
			stream << symbol;
		}
		
		return stream;
	}
} }
