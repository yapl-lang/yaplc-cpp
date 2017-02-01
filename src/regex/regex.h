#pragma once

#include <string>
#include <vector>

namespace regex {
	typedef void (*ErrorHandler)(const std::string &regex,
		const std::string &string,
		const std::string &error);
	
	bool match(const std::string &regex,
		const std::string &string,
		std::vector<std::string> &caps,
		unsigned long capsCount = 0,
		bool ignoreCase = false);
	
	inline bool match(const std::string &regex,
		const std::string &string,
		bool ignoreCase = false) {
		std::vector<std::string> caps;
		
		return match(regex, string, caps, 0, ignoreCase);
	}
	
	void matchAll(const std::string &regex,
		const std::string &string,
		std::vector<std::string *> &caps,
		unsigned long capsCount,
		bool ignoreCase = false);
	
	void setErrorHandler(ErrorHandler handler);
}
