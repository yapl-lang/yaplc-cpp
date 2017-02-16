#include "LogicalError.h"

namespace yaplc { namespace process {
	LogicalError::LogicalError(const std::string &message,
		unsigned long beginLineNumber,
		unsigned long beginColumnNumber,
		unsigned long endLineNumber,
		unsigned long endColumnNumber) :
		PositionalError(CompilingError::Type::Logical,
		message,
		beginLineNumber,
		beginColumnNumber,
		endLineNumber,
		endColumnNumber) {
		
	}
} }
