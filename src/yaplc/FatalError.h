#pragma once

#include "CompilingError.h"

namespace yaplc {
	class FatalError : public CompilingError {
	public:
		FatalError(const std::string &message);
	};
}
