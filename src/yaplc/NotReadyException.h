#pragma once

#include <stdexcept>

namespace yaplc {
	class NotReadyException : public std::logic_error {
	public:
		NotReadyException() :
			std::logic_error("Compiler is not ready.") {}
	};
}
