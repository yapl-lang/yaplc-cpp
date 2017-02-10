#include "PositionalError.h"

namespace yaplc {
	PositionalError::PositionalError(Type type,
		const std::string &message,
		unsigned long beginLineNumber,
		unsigned long beginColumnNumber,
		unsigned long endLineNumber,
		unsigned long endColumnNumber) :
		CompilingError(type, message),
		beginLineNumber(beginLineNumber),
		beginColumnNumber(beginColumnNumber),
		endLineNumber(((endLineNumber == 0) && (endColumnNumber == 0)) ? (beginLineNumber) : (endLineNumber)),
		endColumnNumber(((endLineNumber == 0) && (endColumnNumber == 0)) ? (beginColumnNumber) : (endColumnNumber)),
		isDouble(false) {
		isDouble = (this->beginLineNumber != this->endLineNumber) ||
			(this->beginColumnNumber != this->endColumnNumber);
	}
}
