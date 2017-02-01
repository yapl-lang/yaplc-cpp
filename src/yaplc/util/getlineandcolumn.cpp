#include "getlineandcolumn.h"

namespace yaplc { namespace util {
	bool getlineandcolumn(const std::string &string,
		unsigned long position,
		unsigned long &line,
		unsigned long &column,
		char lineSeparator) {
		line = column = 0;
		
		if (position >= string.size()) {
			return false;
		}
		
		for (unsigned long i = 0; i < position; ++i) {
			++column;
			
			if (string[i] == lineSeparator) {
				column = 0;
				
				++line;
			}
		}
		
		return true;
	}
} }
