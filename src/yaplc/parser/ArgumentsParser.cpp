#include "ArgumentsParser.h"
#include "ExpressionParser.h"
#include "TypeNameParser.h"

namespace yaplc { namespace parser {
	void ArgumentsParser::handle(structure::ArgumentsNode *argumentsNode) {
		skipEmpty();
		push();

		if (get() != '(') {
			cancel();
		}
		skip();
		skipEmpty();

		while (get() != ')') {
			structure::TypeNameNode *argumentType = nullptr;
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

parseEnding:
			switch (get()) {
			case ')':
				break;
			case ',':
				skip();
				break;
			case '=':
				if (value != nullptr) {
					error("Excepted ')', ','. Got '='.");
					cancelFatal();
				}

				skip();

				if (!parse<ExpressionParser>(value = new structure::ExpressionNode(), false)) {
					delete argumentType;
					delete value;

					error("Expected value.");
					cancelFatal();
				}

				goto parseEnding;
			default:
				error(std::string("Excepted ')', ',' or '='. Got '") + get() + "'.");
				cancelFatal();
			}

			argumentsNode->arguments.push_back(std::make_tuple(argumentType, name, value));
		}

		expected(')');
	}
} }
