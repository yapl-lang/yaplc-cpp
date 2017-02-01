#include "regex.h"
#include "slre/slre.h"

#include <iostream>
#include <string.h>

namespace regex {
	ErrorHandler errorHandler;
	
	void regexErrorHandler(const std::string &regex,
		const std::string &string,
		const std::string &error) {
		std::cout
			<< "Regex error ("
			<< error
			<< "):"
			<< std::endl
			<< "\tRegex:"
			<< std::endl
			<< regex
			<< std::endl
			<< "\tString:"
			<< std::endl
			<< string
			<< std::endl;
	}

	void check() {
		if (errorHandler == nullptr) {
			errorHandler = regexErrorHandler;
		}
	}
	
	bool match(const std::string &regex,
		const std::string &string,
		std::vector<std::string> &caps,
		unsigned long capsCount,
		bool ignoreCase) {
		check();
		
		slre_cap *capsArray = (capsCount == 0) ? (nullptr) : (new slre_cap[capsCount]);
		bool result = false;
		
		memset(capsArray, 0, sizeof(capsArray[0]) * capsCount);
		caps.reserve(capsCount);
		
		try {
			auto code = slre_match(regex.c_str(),
				string.c_str(),
				string.size(),
				capsArray,
				capsCount,
				(ignoreCase) ? (SLRE_IGNORE_CASE) : (0));
			
			switch (code) {
			case SLRE_NO_MATCH:
				// It's really not an error
				// errorHandler(regex, string, "NO_MATCH");
				break;
			case SLRE_UNEXPECTED_QUANTIFIER:
				errorHandler(regex, string, "UNEXPECTED_QUANTIFIER");
				break;
			case SLRE_UNBALANCED_BRACKETS:
				errorHandler(regex, string, "UNBALANCED_BRACKETS");
				break;
			case SLRE_INTERNAL_ERROR:
				errorHandler(regex, string, "INTERNAL_ERROR");
				break;
			case SLRE_INVALID_CHARACTER_SET:
				errorHandler(regex, string, "INVALID_CHARACTER_SET");
				break;
			case SLRE_INVALID_METACHARACTER:
				errorHandler(regex, string, "INVALID_METACHARACTER");
				break;
			case SLRE_CAPS_ARRAY_TOO_SMALL:
				errorHandler(regex, string, "CAPS_ARRAY_TOO_SMALL");
				break;
			case SLRE_TOO_MANY_BRANCHES:
				errorHandler(regex, string, "TOO_MANY_BRANCHES");
				break;
			case SLRE_TOO_MANY_BRACKETS:
				errorHandler(regex, string, "TOO_MANY_BRACKETS");
				break;
			}
			
			if (code > 0) {
				result = true;
				
				for (unsigned long i = 0; i < capsCount; ++i) {
					if (capsArray[i].len == 0) {
						caps.push_back("");
					} else {
						caps.push_back(std::string(capsArray[i].ptr, capsArray[i].len));
					}
				}
			}
		} catch (std::exception &e) {
			errorHandler(regex, string, e.what());
			result = false;
		}
		
		delete[] capsArray;
		
		return result;
	}
	
	void matchAll(const std::string &regex,
		const std::string &string,
		std::vector<std::string *> &caps,
		unsigned long capsCount,
		bool ignoreCase) {
		check();
		
		slre_cap *capsArray = (capsCount == 0) ? (nullptr) : (new slre_cap[capsCount]);
		unsigned long scanned = 0;
		
		while (true) {
			try {
				auto code = slre_match(regex.c_str(),
					string.c_str() + scanned,
					string.size() - scanned,
					capsArray,
					capsCount,
					(ignoreCase) ? (SLRE_IGNORE_CASE) : (0));
				
				switch (code) {
				case SLRE_NO_MATCH:
					// It's really not an error
					// errorHandler(regex, string, "NO_MATCH");
					break;
				case SLRE_UNEXPECTED_QUANTIFIER:
					errorHandler(regex, string, "UNEXPECTED_QUANTIFIER");
					break;
				case SLRE_UNBALANCED_BRACKETS:
					errorHandler(regex, string, "UNBALANCED_BRACKETS");
					break;
				case SLRE_INTERNAL_ERROR:
					errorHandler(regex, string, "INTERNAL_ERROR");
					break;
				case SLRE_INVALID_CHARACTER_SET:
					errorHandler(regex, string, "INVALID_CHARACTER_SET");
					break;
				case SLRE_INVALID_METACHARACTER:
					errorHandler(regex, string, "INVALID_METACHARACTER");
					break;
				case SLRE_CAPS_ARRAY_TOO_SMALL:
					errorHandler(regex, string, "CAPS_ARRAY_TOO_SMALL");
					break;
				case SLRE_TOO_MANY_BRANCHES:
					errorHandler(regex, string, "TOO_MANY_BRANCHES");
					break;
				case SLRE_TOO_MANY_BRACKETS:
					errorHandler(regex, string, "TOO_MANY_BRACKETS");
					break;
				}
				
				if (code <= 0) {
					break;
				}
				
				if ((scanned = scanned + code) > string.size()) {
					break;
				}
				
				if (capsCount != 0) {
					auto capsSet = new std::string[capsCount];
					
					for (unsigned long i = 0; i < capsCount; ++i) {
						std::string cap(capsArray[i].ptr, capsArray[i].len);
						
						capsSet[i] = cap;
					}
					
					caps.push_back(capsSet);
				}
			} catch (...) {
				
			}
		}
		
		delete[] capsArray;
	}
	
	void setErrorHandler(ErrorHandler handler) {
		errorHandler = handler;
	}
}
