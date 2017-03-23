#pragma once

#include "Node.h"
#include "ArgumentsNode.h"

namespace yaplc { namespace structure {
	class MethodMemberNode : public Node {
	public:
		ArgumentsNode *arguments;
		ExpressionNode *body;

	public:
		MethodMemberNode() :
			Node(),
			arguments(new ArgumentsNode()),
			body(new ExpressionNode()) {

		}

		virtual ~MethodMemberNode() {
			delete body;
			delete arguments;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(arguments)
			NODE_PROP_AUTO(body)
		)

	public:
		virtual void load(const binstream &stream) {
			Node::load(stream);

			NodeFactory::loadNode(stream, arguments);
			NodeFactory::loadNode(stream, body);
		}

		virtual void save(binstream &stream) const {
			Node::save(stream);

			NodeFactory::saveNode(stream, arguments);
			NodeFactory::saveNode(stream, body);
		}
	};
} }
