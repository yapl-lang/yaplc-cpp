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
					delete operatorNode;
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
		auto begin = position();
		expected('(', true, false, false);

		auto group = new structure::ExpressionNode();
		group->setBegin(begin);
		if (!parse<ExpressionParser>(group, true)) {
			delete group;
			error("Expression expected.");
			cancelFatal();
		}
		parentNode->add(group);

		expected(')');
		end(group);
	}
} }
