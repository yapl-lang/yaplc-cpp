#pragma once

#include "container.h"
#include <string>
#include <vector>

namespace yaplc { namespace structure {
	class MemberNode : public Container {
	public:
		std::string type;
		std::vector<std::string> modifiers;
	};
} }
