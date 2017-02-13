#include "ExpressionParser.h"
#include "NumberExpressionParser.h"
#include "StringExpressionParser.h"
#include "NullExpressionParser.h"
#include "TypeExpressionParser.h"
#include "OperatorParser.h"
#include "BracketExpressionParser.h"

namespace yaplc { namespace parser {
	void ExpressionParser::handle(structure::ExpressionNode *node, bool dynamic) {
		cancelIfEnd();

		bool anyParsed = false;

		while (true) {
			if ((!parse<BracketExpressionParser>(node)) &&
				(!parse<NumberExpressionParser>(node)) &&
				(!parse<StringExpressionParser>(node)) &&
				(!parse<NullExpressionParser>(node)) &&
				(!parse<TypeExpressionParser>(node))) {
				break;
			}

			anyParsed = true;

			if (!parse<OperatorParser>(node)) {
				break;
			}
		}

		if (!anyParsed) {
			cancel();
		}

		// TODO: Check dynamic
	}
} }
