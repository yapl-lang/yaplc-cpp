#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class CharNode : public Node {
	public:
		std::string value;
		
		NODE_PROPS(
			NODE_PROP(value, stream << value)
		)
	};
} }
