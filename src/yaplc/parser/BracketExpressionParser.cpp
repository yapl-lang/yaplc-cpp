#include "BracketExpressionParser.h"
#include "OperatorParser.h"
#include "ExpressionParser.h"

namespace yaplc { namespace parser {
	void BracketExpressionParser::handle(structure::ExpressionNode *parentNode) {
		cancelIfEnd();

		save();
		if (parse<OperatorParser>(parentNode)) {
			auto it = --parentNode->end();

			if (auto operatorNode = dynamic_cast<structure::OperatorNode *>(*it)) {
				if (operatorNode->type == structure::OperatorNode::Type::Call) {
					parentNode->remove(operatorNode);
					restore();
				} else {
					norestore();
				}
			} else {
				norestore();
			}
		} else {
			norestore();
		}

		push();

		skipEmpty();
		expected('(', true, false, false);

		auto group = new structure::ExpressionNode();
		if (!parse<ExpressionParser>(group, true)) {
			error("Expression expected.");
			cancelFatal();
		}
		parentNode->add(group);

		expected(')');
	}
} }
