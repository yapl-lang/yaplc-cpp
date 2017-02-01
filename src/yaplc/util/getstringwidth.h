#pragma once

#include <string>

namespace yaplc { namespace util {
	unsigned long getstringwidth(const std::string &string);
	unsigned long getstringwidth(const std::string &string, unsigned long column);
} }
