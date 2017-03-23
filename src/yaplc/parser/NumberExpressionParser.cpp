#include "NumberExpressionParser.h"
#include "OperatorParser.h"
#include "yaplc/structure/NumberNode.h"

namespace yaplc { namespace parser {
	std::regex NumberExpressionParser::NumberRegex{"([0-9]*(\\.[0-9]*)?)"};

	void NumberExpressionParser::handle(structure::ExpressionNode *parentNode) {
		cancelIfEnd();

		parse<OperatorParser>(parentNode);
		push();

		std::smatch match;
		if (!get(NumberRegex, match)) {
			cancel();
		}

		auto number = match[1].str() + (match[2].length() == 0 ? "" : ("" + match[2].str()));

		if (number == ".") {
			cancel();
		}

		auto node = new structure::NumberNode();
		node->value = number;
		parentNode->add(node);
	}
} }
