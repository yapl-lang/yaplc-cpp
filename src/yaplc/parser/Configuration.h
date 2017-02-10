#pragma once

#include <string>

namespace yaplc { namespace parser {
	struct Configuration {
		unsigned long position;
		std::string const *code;
		
		Configuration() :
			position(0) {
			
		}
		
		Configuration &operator =(const Configuration &value) {
			code = value.code;
			position = value.position;
			
			return *this;
		}
	};
} }
