#pragma once

#include "Node.h"
#include "TypeNameNode.h"
#include "ExpressionNode.h"

namespace yaplc { namespace structure {
	class VariableMemberNode : public Node {
	public:
		ExpressionNode *defaultValue;

	public:
		inline VariableMemberNode() :
			Node(),
			defaultValue(new ExpressionNode()) {

		}

		virtual ~VariableMemberNode() {
			delete defaultValue;
		}
		
		NODE_PROPS(
			NODE_PROP_AUTO(defaultValue);
		)
	};
} }
