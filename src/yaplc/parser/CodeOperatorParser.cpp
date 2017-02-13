#include "CodeOperatorParser.h"
#include "CodeVariableParser.h"
#include "ExpressionParser.h"
#include "IfOperatorParser.h"
#include "WhileOperatorParser.h"

namespace yaplc { namespace parser {
	void CodeOperatorParser::handle(structure::ExpressionNode *parentNode) {
		skipEmpty();

		if (parse<IfOperatorParser>(parentNode)) {
			return;
		}
		parentNode->clear();

		if (parse<WhileOperatorParser>(parentNode)) {
			return;
		}
		parentNode->clear();

		if (parse<CodeVariableParser>(parentNode)) {
			return;
		}
		parentNode->clear();

		if (parse<ExpressionParser>(parentNode, true)) {
			return;
		}
		parentNode->clear();

		cancel();
	}
} }
