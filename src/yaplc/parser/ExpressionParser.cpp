#include "ExpressionParser.h"
#include "NumberExpressionParser.h"
/*#include "stringexpressionparser.h"
#include "arrayexpressionparser.h"
#include "objectexpressionparser.h"*/

namespace yaplc { namespace parser {
	void ExpressionParser::handle(structure::ValueNode *node, bool dynamic) {
		cancelIfEnd();
		
		if (parse<NumberExpressionParser>(node)) {
			return;
		}
		
		if (dynamic) {
			
		}
		
		cancel();
	}
} }
