#include "CompilingError.h"

namespace yaplc {
	CompilingError::CompilingError(Type type,
		std::string message) :
		type(type),
		message(message) {
		
	}
}
