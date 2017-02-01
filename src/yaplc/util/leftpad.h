#pragma once

#include <string>
#include <sstream>

namespace yaplc { namespace util {
	std::string leftpad(const std::string &string, unsigned long count, char symbol = ' ');
	std::ostream &leftpad(std::ostream &stream, unsigned long count, char symbol = ' ');
} }
