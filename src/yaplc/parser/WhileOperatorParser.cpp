#include "WhileOperatorParser.h"
#include "CodeOperatorParser.h"
#include "CodeParser.h"
#include "ExpressionParser.h"
#include "yaplc/structure/WhileOperatorNode.h"

namespace yaplc { namespace parser {
	void WhileOperatorParser::handle(structure::ExpressionNode *parentNode) {
		skipOrCancel("while");
		skipEmpty();

		bool hasBrace = get() == '(';

		auto node = new structure::WhileOperatorNode();
		parentNode->add(node);

		if (!parse<ExpressionParser>(node->condition, true)) {
			error("Expression expected.");
			cancelFatal();
		}

		bool hasThen = skip("do");
		skipEmpty();

		if (skipChar('{')) {
			parse<CodeParser>(node->expression);
			expected('}');
		} else if ((hasBrace) || (hasThen)) {
			auto codeOperator = new structure::ExpressionNode();

			if (!parse<CodeOperatorParser>(codeOperator)) {
				delete codeOperator;

				error("Expected operator.");
				cancelFatal();
			}

			node->expression->add(codeOperator);

			expected(';');
		} else {
			error("Expected 'do'.");
			cancelFatal();
		}
	}
} }
