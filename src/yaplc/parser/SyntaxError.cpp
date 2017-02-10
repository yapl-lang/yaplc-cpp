#include "SyntaxError.h"

namespace yaplc { namespace parser {
	SyntaxError::SyntaxError(const std::string &message,
		unsigned long beginLineNumber,
		unsigned long beginColumnNumber,
		unsigned long endLineNumber,
		unsigned long endColumnNumber) :
		PositionalError(CompilingError::Type::Syntax,
		message,
		beginLineNumber,
		beginColumnNumber,
		endLineNumber,
		endColumnNumber) {
		
	}
} }
