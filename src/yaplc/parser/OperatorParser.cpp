#include "OperatorParser.h"
#include "ExpressionParser.h"

namespace yaplc { namespace parser {
	std::regex OperatorParser::WhitespaceRegex{"^[ \t]*$"};

	void OperatorParser::handle(structure::Listable *parentNode) {
		auto position1 = position();
		save();
		skipEmpty();
		begin();
		push();

		for (auto op : structure::OperatorNode::Operators) {
			if (get(op.first.size()) == op.first) {
				skip(op.first.size());

				switch (op.second) {
				case structure::OperatorNode::Type::Call: {
					auto node = new structure::OperatorNode();
					node->type = op.second;
					parentNode->add(node);

					parseArguments(node);

					expected(')');
					end(node);
					break;
				}
				case structure::OperatorNode::Type::Array: {
					auto node = new structure::OperatorNode();
					node->type = op.second;
					parentNode->add(node);

					parseArguments(node);

					expected(']');
					end(node);
					break;
				}
				default: {
					auto node = new structure::OperatorNode();
					node->type = op.second;
					parentNode->add(node);
					end(node);
				}
				}

				return;
			}
		}

		auto position2 = position();
		begin(position1);

		if ((position1 != position2) && (std::regex_match(configuration.code->substr(position1, position2 - position1), WhitespaceRegex))) {
			auto node = new structure::OperatorNode();
			node->type = structure::OperatorNode::Type::Whitespace;
			parentNode->add(node);
			end(node);

			parseArguments(node);
		}

		cancel();
	}

	void OperatorParser::parseArguments(structure::OperatorNode *node) {
		while (true) {
			if (!parse<ExpressionParser>(node->expression, true)) {
				break;
			}

			skipEmpty();

			switch (get()) {
			case ',':
				skip();
				break;
			default:
				goto done;
			}
		}

done:;
	}
} }
