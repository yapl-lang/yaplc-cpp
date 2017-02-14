#pragma once

#include "Node.h"
#include "TypeNameNode.h"

namespace yaplc { namespace structure {
	class TypeExpressionNode : public Node {
	public:
		TypeNameNode *type;
		Node *child;

	public:
		inline TypeExpressionNode() :
			Node(),
			type(nullptr),
			child(nullptr) {
			
		}
		
		virtual ~TypeExpressionNode() {
			delete type;
			delete child;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(type)
			NODE_PROP_AUTO(child)
		)
	};
} }
