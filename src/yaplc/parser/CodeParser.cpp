#include "CodeParser.h"
#include "CodeOperatorParser.h"

namespace yaplc { namespace parser {
	void CodeParser::handle(structure::ExpressionNode *parentNode) {
		while (get() != '}') {
			auto codeOperator = new structure::ExpressionNode();

			if (!parse<CodeOperatorParser>(codeOperator)) {
				delete codeOperator;

				break;
			}

			parentNode->add(codeOperator);
			skipEmpty();

			if (get() == ';') {
				skip();
				skipEmpty();
			}
		}
	}
} }
