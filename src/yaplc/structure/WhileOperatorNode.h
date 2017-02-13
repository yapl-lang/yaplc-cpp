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
	};
} }
