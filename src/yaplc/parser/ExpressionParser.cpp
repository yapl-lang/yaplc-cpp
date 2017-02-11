#include "ExpressionParser.h"
#include "NumberExpressionParser.h"
#include "OperatorParser.h"

namespace yaplc { namespace parser {
	void ExpressionParser::handle(structure::ExpressionNode *node, bool dynamic) {
		cancelIfEnd();

		parse<OperatorParser>(node);

		while (true) {
			if ((!parse<NumberExpressionParser>(node))) {
				break;
			}

			if (!parse<OperatorParser>(node)) {
				break;
			}
		}

		// TODO: Check dynamic
	}
} }
