#pragma once

#include "node.h"

namespace yaplc { namespace structure {
	class ImportNode : public Node {
	public:
		bool isStatic;

		NODE_PROPS(
			NODE_PROP(isStatic, stream << ((isStatic) ? ("true") : ("false")))
		)
	};
} }
