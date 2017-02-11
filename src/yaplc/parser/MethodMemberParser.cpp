#include "MethodMemberParser.h"
#include "TypeNameParser.h"
#include "yaplc/structure/MethodMemberNode.h"
#include <memory>

namespace yaplc { namespace parser {
	void MethodMemberParser::handle(structure::MemberNode *parentNode, bool withoutBody) {
		skipEmpty();
		push();

		if (get() != '(') {
			cancel();
		}
		skip();
		skipEmpty();

		auto methodMemberNode = new structure::MethodMemberNode();
		parentNode->set(methodMemberNode);

		while (get() != ')') {
			structure::TypeNameNode *argumentType;
			std::string name;
			structure::ExpressionNode *value = nullptr;

			if (!parse<TypeNameParser>(&argumentType)) {
				error("Type name expected.");
				cancelFatal();
			}

			if (!getWord(name)) {
				delete argumentType;

				error("Argument name expected.");
				cancelFatal();
			}

			skipEmpty();

			switch (get()) {
			case ')':
				break;
			case ',':
				skip();
				break;
			case '=':
				// TODO: Parse default value.
				break;
			default:
				error(std::string("Excepted ')', ',' or '='. Got '") + get() + "'.");
			}

			methodMemberNode->arguments->arguments.push_back(std::make_tuple(argumentType, name, value));
		}

		skipEmpty();
		expected(')');

		skip();
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

				// TODO: Parse code

				skipEmpty();
				expected('}');
				break;
			default:
				error(std::string("Expected ';' or '{'. Got '") + get() + "'.");
			}
		}
	}
} }
