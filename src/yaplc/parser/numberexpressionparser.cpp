#include "numberexpressionparser.h"
#include "yaplc/structure/ariphmeticexpression.h"

namespace yaplc { namespace parser {
	void NumberExpressionParser::handle(structure::Node **node) {
		cancelIfEnd();
		
		std::string number;
		
		if (!get("([0-9]*(\\.[0-9]*)?)", {&number, nullptr})) {
			cancel();
		}
		
		auto ariphmeticNode = new structure::AriphmeticExpression();
		ariphmeticNode->value = number;
		*node = ariphmeticNode;
	}
} }
