#pragma once

#include "../PositionalError.h"

namespace yaplc { namespace parser {
	class SyntaxError : public PositionalError {
	public:
		SyntaxError(const std::string &message,
			unsigned long beginLineNumber,
			unsigned long beginColumnNumber,
			unsigned long endLineNumber = 0,
			unsigned long endColumnNumber = 0);
	};
} }
