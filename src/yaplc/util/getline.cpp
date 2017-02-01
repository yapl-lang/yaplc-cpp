#include "getline.h"

namespace yaplc { namespace util {
	std::string getline(const std::string &string, unsigned long index, char lineSeparator) {
		unsigned long i = 0;
		
		if (index != 0) {
			for (i = 0; i < string.size(); ++i) {
				if ((string[i] == lineSeparator) && (--index == 0)) {
					break;
				}
			}
			
			++i;
		}
		
		for (unsigned long j = i; j < string.size(); ++j) {
			if (string[j] == lineSeparator) {
				return string.substr(i, j - i);
			}
		}
		
		if (i != string.size()) {
			return string.substr(i);
		}
		
		return "";
	}
} }
