#include "MethodMemberParser.h"
#include "ExpressionParser.h"
#include "CodeOperatorParser.h"
#include "TypeNameParser.h"
#include "yaplc/structure/MethodMemberNode.h"
#include "CodeParser.h"
#include "ArgumentsParser.h"

namespace yaplc { namespace parser {
	void MethodMemberParser::handle(structure::MemberNode *parentNode, bool withoutBody) {
		auto methodMemberNode = new structure::MethodMemberNode();
		parentNode->set(methodMemberNode);

		if (!parse<ArgumentsParser>(methodMemberNode->arguments)) {
			cancel();
		}

		skipEmpty();

		if (withoutBody) {
			expected(';', false);
		} else {
			switch (get()) {
			case ';':
				skip();
				break;
			case '{':
				skip();

				parse<CodeParser>(methodMemberNode->body);

				skipEmpty();
				expected('}');
				break;
			default:
				error(std::string("Expected ';' or '{'. Got '") + get() + "'.");
			}
		}
	}
} }
