#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class NumberNode : public Node {
	public:
		std::string value;

		NODE_PROPS(
			NODE_PROP(value, stream << value)
		)
	};
} }
