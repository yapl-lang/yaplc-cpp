#pragma once

#include <stdexcept>

namespace yaplc { namespace parser {
	class CancelParseException : public std::exception {
	public:
		CancelParseException() :
			std::exception() {
			
		}
	};
} }
