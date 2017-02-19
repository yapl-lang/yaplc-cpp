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

	public:
		virtual void load(const binstream::stream &stream) {
			Node::load(stream);

			NodeFactory::loadNode(stream, defaultValue);
		}

		virtual void save(binstream::stream &stream) const {
			Node::save(stream);

			NodeFactory::saveNode(stream, defaultValue);
		}
	};
} }
