#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class DoWhileOperatorNode : public Node {
	public:
		ExpressionNode *condition;
		ExpressionNode *expression;

	public:
		inline DoWhileOperatorNode() :
			Node(),
			condition(new ExpressionNode()),
			expression(new ExpressionNode()) {}

		virtual ~DoWhileOperatorNode() {
			delete expression;
			delete condition;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(condition)
			NODE_PROP_AUTO(expression)
		)
	};
} }
