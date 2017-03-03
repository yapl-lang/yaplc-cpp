#include "OperatorMemberParser.h"
#include "ArgumentsParser.h"
#include "CodeParser.h"
#include "yaplc/structure/OperatorNode.h"
#include "yaplc/structure/OperatorMemberNode.h"

namespace yaplc { namespace parser {
	void OperatorMemberParser::handle(structure::MemberNode *parentNode, bool withoutBody) {
		if (parentNode->getName() != "operator") {
			cancel();
		}

		auto type = (structure::OperatorNode::Type)-1;

		save();
		skipEmpty();
		begin();
		push();

		if (skip("[]=")) {
			type = structure::OperatorNode::Type::ArraySet;
		} else {
			for (auto op : structure::OperatorNode::Operators) {
				if (get(op.first.size()) == op.first) {
					skip(op.first.size());

					switch (op.second) {
					case structure::OperatorNode::Type::Call:
						type = op.second;
						expected(')');
						break;
					case structure::OperatorNode::Type::Array:
						type = op.second;
						expected(']');
						break;
					default:
						type = op.second;
					}
				}
			}

			if (type == (structure::OperatorNode::Type)-1) {
				restore();
				error("Undefined operator type");
				cancelFatal();
			}
		}

		norestore();

		auto methodMemberNode = new structure::OperatorMemberNode();
		parentNode->set(methodMemberNode);

		if (!parse<ArgumentsParser>(methodMemberNode->arguments)) {
			error("Arguments expected.");
			cancelFatal();
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
