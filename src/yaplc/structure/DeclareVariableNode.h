#pragma once

#include "Node.h"
#include "TypeNameNode.h"
#include "ExpressionNode.h"

namespace yaplc { namespace structure {
	class DeclareVariableNode : public Node {
	public:
		TypeNameNode *type;
		std::string name;
		ExpressionNode *value;

	public:
		inline DeclareVariableNode() :
			Node(),
			type(nullptr),
			value(nullptr) {}

		virtual ~DeclareVariableNode() {
			delete value;
			delete type;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(type)
			NODE_PROP(name, stream << name)
			NODE_PROP_AUTO(value)
		)
	};
} }
