#pragma once

#include "Node.h"
#include "TypeNameNode.h"

namespace yaplc { namespace structure {
	class TypeExpressionNode : public Node {
	public:
		TypeNameNode *type;
		Node *child;

	public:
		virtual ~TypeExpressionNode() {
			delete type;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(type)
			NODE_PROP_AUTO(child)
		)
	};
} }
