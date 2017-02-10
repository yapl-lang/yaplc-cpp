#pragma once

#include "CompilingError.h"

namespace yaplc {
	class PositionalError : public CompilingError {
	public:
		unsigned long beginLineNumber, beginColumnNumber;
		unsigned long endLineNumber, endColumnNumber;
		bool isDouble;
		
	public:
		PositionalError(Type type,
			const std::string &message,
			unsigned long beginLineNumber,
			unsigned long beginColumnNumber,
			unsigned long endLineNumber = 0,
			unsigned long endColumnNumber = 0);
	};
}
