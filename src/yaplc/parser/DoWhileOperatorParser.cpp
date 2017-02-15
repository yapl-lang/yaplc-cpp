#include "DoWhileOperatorParser.h"
#include "CodeOperatorParser.h"
#include "CodeParser.h"
#include "ExpressionParser.h"
#include "yaplc/structure/DoWhileOperatorNode.h"

namespace yaplc { namespace parser {
	void DoWhileOperatorParser::handle(structure::ExpressionNode *parentNode) {
		skipOrCancel("do");
		skipEmpty();

		auto node = new structure::DoWhileOperatorNode();
		parentNode->add(node);

		skipEmpty();

		if (skipChar('{')) {
			parse<CodeParser>(node->expression);
			expected('}');
		} else {
			auto codeOperator = new structure::ExpressionNode();

			if (!parse<CodeOperatorParser>(codeOperator)) {
				delete codeOperator;

				error("Expected operator.");
				cancelFatal();
			}

			node->expression->add(codeOperator);

			expected(';');
		}

		skipOrFail("while", "Expected 'while'.");

		if (!parse<ExpressionParser>(node->condition, true)) {
			error("Expression expected.");
			cancelFatal();
		}

		expected(';');
	}
} }
