#include "NumberExpressionParser.h"
#include "yaplc/structure/ArithmeticExpressionNode.h"

namespace yaplc { namespace parser {
	void NumberExpressionParser::handle(structure::ValueNode *node) {
		cancelIfEnd();
		
		std::string number;
		
		if (!get("([0-9]*(\\.[0-9]*)?)", {&number, nullptr})) {
			cancel();
		}
		
		auto arithmeticNode = new structure::ArithmeticExpression();
		arithmeticNode->value = number;
		node->set(arithmeticNode);
	}
} }
