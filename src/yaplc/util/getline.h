#pragma once

#include <string>

namespace yaplc { namespace util {
	std::string getline(const std::string &string, unsigned long index, char lineSeparator = '\n');
} }
