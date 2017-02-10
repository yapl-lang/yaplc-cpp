#include "FatalError.h"

namespace yaplc {
	FatalError::FatalError(const std::string &message) :
		CompilingError(CompilingError::Type::Fatal, message) {
		
	}
}
