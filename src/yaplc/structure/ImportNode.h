#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class ImportNode : public Node {
	public:
		bool isStatic;
		std::string target;

		NODE_PROPS(
			NODE_PROP(isStatic, stream << ((isStatic) ? ("true") : ("false")))
			NODE_PROP(target, stream << target)
		)
	};
} }
