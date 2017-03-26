#include "VariableMemberParser.h"
#include "ExpressionParser.h"
#include "yaplc/structure/VariableMemberNode.h"

namespace yaplc { namespace parser {
	void VariableMemberParser::handle(structure::MemberNode *parentNode) {
		skipEmpty();
		begin();

		/*if (parentNode->getName() == "size") {
			error("");
			cancelFatal();
		}*/

		switch (get()) {
		case ';': {
			skip();

			auto variableMemberNode = new structure::VariableMemberNode();
			parentNode->set(variableMemberNode);
			break;
		}
		case '=': {
			skip();

			auto variableMemberNode = new structure::VariableMemberNode();
			parentNode->set(variableMemberNode);

			if (!parse<ExpressionParser>(variableMemberNode->defaultValue, false)) {
				error("Expected value.");
				cancelFatal();
			}

			skipEmpty();

			expected(';', false);
			break;
		}
		default:
			error("Expected ';' or '=' after variable member.");
			cancelFatal();
		}

		end(parentNode);
	}
} }
