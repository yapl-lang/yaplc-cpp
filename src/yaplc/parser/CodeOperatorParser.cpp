#include "CodeOperatorParser.h"
#include "CodeVariableParser.h"

namespace yaplc { namespace parser {
	void CodeOperatorParser::handle(structure::ExpressionNode *parentNode) {
		if (parse<CodeVariableParser>(parentNode)) {
			return;
		}
		parentNode->clear();

		cancel();
	}
} }
