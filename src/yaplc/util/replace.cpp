#include "replace.h"

namespace yaplc { namespace util {
	std::string replace(const std::string &string, const std::string &source, const std::string &target) {
		if (source.empty()) {
			return string;
		}

		auto result = string;
		size_t start_pos = 0;

		while ((start_pos = result.find(source, start_pos)) != std::string::npos) {
			result.replace(start_pos, source.length(), target);
			start_pos += target.length();
		}

		return result;
	}
} }
