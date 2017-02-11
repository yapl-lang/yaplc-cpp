#include "NumberExpressionParser.h"
#include "OperatorParser.h"
#include "yaplc/structure/NumberNode.h"

namespace yaplc { namespace parser {
	void NumberExpressionParser::handle(structure::ExpressionNode *parentNode) {
		cancelIfEnd();
		parse<OperatorParser>(parentNode);
		
		std::string number;
		if (!get("([0-9]*(\\.[0-9]*)?)", {&number, nullptr})) {
			cancel();
		}

		auto node = new structure::NumberNode();
		node->value = number;
		parentNode->add(node);
	}
} }
