#pragma once

#include <string>

namespace yaplc { namespace util {
	bool getlineandcolumn(const std::string &string,
		unsigned long position,
		unsigned long &line,
		unsigned long &column,
		char lineSeparator = '\n');
} }
