#include "getstringwidth.h"

namespace yaplc { namespace util {
	unsigned long getstringwidth(const std::string &string) {
		return getstringwidth(string, string.length());
	}
	
	unsigned long getstringwidth(const std::string &string, unsigned long column) {
		unsigned long width = column;
		
		for (unsigned long i = 0; i < column; ++i) {
			switch (string[i]) {
			case '\t':
				width += 3;
				break;
			}
		}
		
		return width;
	}
} }
