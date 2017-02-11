#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class StringNode : public Node {
	public:
		std::string value;
	};
} }
