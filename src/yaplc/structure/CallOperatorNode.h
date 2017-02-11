#pragma once

#include "OperatorNode.h"
#include "ExpressionNode.h"

namespace yaplc { namespace structure {
	class CallOperatorNode : public OperatorNode {
	public:
		ExpressionNode *expression;

	public:
		inline CallOperatorNode() :
			expression(new ExpressionNode()) {}
		virtual ~CallOperatorNode() {
			delete expression;
		}

		NODE_PROPS_PARENT(
			NODE_PROP_AUTO(expression)
		, OperatorNode)
	};
} }
