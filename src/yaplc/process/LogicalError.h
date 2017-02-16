#pragma once

#include "../PositionalError.h"

namespace yaplc { namespace process {
	class LogicalError : public PositionalError {
	public:
		LogicalError(const std::string &message,
			unsigned long beginLineNumber,
			unsigned long beginColumnNumber,
			unsigned long endLineNumber = 0,
			unsigned long endColumnNumber = 0);
	};
} }
