#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class WhileOperatorNode : public Node {
	public:
		ExpressionNode *condition;
		ExpressionNode *expression;

	public:
		inline WhileOperatorNode() :
			Node(),
			condition(new ExpressionNode()),
			expression(new ExpressionNode()) {}

		virtual ~WhileOperatorNode() {
			delete expression;
			delete condition;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(condition)
			NODE_PROP_AUTO(expression)
		)

	public:
		virtual void load(const binstream::stream &stream) {
			Node::load(stream);

			NodeFactory::loadNode(stream, condition);
			NodeFactory::loadNode(stream, expression);
		}

		virtual void save(binstream::stream &stream) const {
			Node::save(stream);

			NodeFactory::saveNode(stream, condition);
			NodeFactory::saveNode(stream, expression);
		}
	};
} }
