#pragma once

#include "node.h"
#include "typereferencenode.h"
#include "valuenode.h"

namespace yaplc { namespace structure {
	class VariableMemberNode : public Node {
	public:
		ValueNode *defaultValue;

	public:
		inline VariableMemberNode() :
			Node(),
			defaultValue(new ValueNode()) {

		}

		virtual ~VariableMemberNode() {
			delete defaultValue;
		}
		
		NODE_PROPS(
			NODE_PROP_AUTO(defaultValue);
		)
	};
} }
