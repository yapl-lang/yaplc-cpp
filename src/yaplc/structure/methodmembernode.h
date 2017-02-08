#pragma once

#include "node.h"
#include "argumentsnode.h"

namespace yaplc { namespace structure {
	class MethodMemberNode : public Node {
	public:
		ArgumentsNode *arguments;

	};
} }
