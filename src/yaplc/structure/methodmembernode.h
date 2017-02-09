#pragma once

#include "node.h"
#include "argumentsnode.h"

namespace yaplc { namespace structure {
	class MethodMemberNode : public Node {
	public:
		ArgumentsNode *arguments;

	public:
		MethodMemberNode() :
			Node(),
			arguments(new ArgumentsNode()) {

		}

		virtual ~MethodMemberNode() {
			delete arguments;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(arguments)
		)
	};
} }
