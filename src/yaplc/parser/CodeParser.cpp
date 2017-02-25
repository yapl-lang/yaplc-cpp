#include "CodeParser.h"
#include "CodeOperatorParser.h"
#include "SpecialParser.h"

namespace yaplc { namespace parser {
	void CodeParser::handle(structure::ExpressionNode *parentNode) {
		while (get() != '}') {
			if (parse<SpecialParser>(parentNode)) {
				continue;
			}

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
