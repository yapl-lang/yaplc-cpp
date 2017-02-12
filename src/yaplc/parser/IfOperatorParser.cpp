#include "IfOperatorParser.h"
#include "ExpressionParser.h"
#include "yaplc/structure/IfOperatorNode.h"
#include "CodeParser.h"
#include "CodeOperatorParser.h"

namespace yaplc { namespace parser {
	void IfOperatorParser::handle(structure::ExpressionNode *parentNode) {
		skipOrCancel("if");

		auto node = new structure::IfOperatorNode();
		parentNode->add(node);

		if (!parse<ExpressionParser>(node->condition, true)) {
			error("Expression expected.");
			cancelFatal();
		}

		skipEmpty();

		if (skipChar('{')) {
			parse<CodeParser>(node->trueExpression);
			expected('}');
		} else {
			auto codeOperator = new structure::ExpressionNode();

			if (!parse<CodeOperatorParser>(node->trueExpression)) {
				delete codeOperator;

				error("Expected operator.");
				cancelFatal();
			}
		}
	}
} }
