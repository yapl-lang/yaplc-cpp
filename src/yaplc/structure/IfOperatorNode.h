#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class IfOperatorNode : public Node {
	public:
		ExpressionNode *condition;
		ExpressionNode *trueExpression;
		ExpressionNode *falseExpression;

	public:
		inline IfOperatorNode() :
			Node(),
			condition(new ExpressionNode()),
			trueExpression(new ExpressionNode()),
			falseExpression(new ExpressionNode()) {}

		virtual ~IfOperatorNode() {
			delete falseExpression;
			delete trueExpression;
			delete condition;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(condition)
			NODE_PROP_AUTO(trueExpression)
			NODE_PROP_AUTO(falseExpression)
		)

	public:
		virtual void load(const binstream &stream) {
			Node::load(stream);

			NodeFactory::loadNode(stream, condition);
			NodeFactory::loadNode(stream, trueExpression);
			NodeFactory::loadNode(stream, falseExpression);
		}

		virtual void save(binstream &stream) const {
			Node::save(stream);

			NodeFactory::saveNode(stream, condition);
			NodeFactory::saveNode(stream, trueExpression);
			NodeFactory::saveNode(stream, falseExpression);
		}
	};
} }
