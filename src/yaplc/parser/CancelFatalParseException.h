#pragma once

#include <stdexcept>

namespace yaplc { namespace parser {
	class CancelFatalParseException : public std::exception {
	public:
		CancelFatalParseException() :
			std::exception() {
			
		}
	};
} }
