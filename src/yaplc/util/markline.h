#pragma once

#include <string>

namespace yaplc { namespace util {
	std::string markline(const std::string &string,
		unsigned long line, unsigned long column);
	
	std::string markline(const std::string &string,
		unsigned long line1, unsigned long column1,
		unsigned long line2, unsigned long column2);
	
	inline std::string markline(const std::string &string,
		unsigned long line, unsigned long column1, unsigned long column2) {
		return markline(string, line, column1, line, column2);
	}
} }
