#include "OperatorParser.h"
#include "ExpressionParser.h"
#include "regex/regex.h"

namespace yaplc { namespace parser {
	void OperatorParser::handle(structure::Listable *parentNode) {
		static std::vector<std::pair<std::string, structure::OperatorNode::Type>> operators = {
			{"==", structure::OperatorNode::Type::Equality},
			{"!=", structure::OperatorNode::Type::Inequality},

			{"+=", structure::OperatorNode::Type::AddSet},
			{"-=", structure::OperatorNode::Type::SubtractSet},
			{"*=", structure::OperatorNode::Type::MultiplySet},
			{"/=", structure::OperatorNode::Type::DivideSet},

			{"++", structure::OperatorNode::Type::Increment},
			{"--", structure::OperatorNode::Type::Decrement},
			{"=", structure::OperatorNode::Type::Set},

			{"+", structure::OperatorNode::Type::Add},
			{"-", structure::OperatorNode::Type::Subtract},
			{"*", structure::OperatorNode::Type::Multiply},
			{"/", structure::OperatorNode::Type::Divide},

			{"%", structure::OperatorNode::Type::Module},

			{">=", structure::OperatorNode::Type::MoreEqual},
			{"<=", structure::OperatorNode::Type::LessEqual},

			{">", structure::OperatorNode::Type::More},
			{"<", structure::OperatorNode::Type::Less},

			{"!", structure::OperatorNode::Type::Not},
			{"&&", structure::OperatorNode::Type::And},
			{"||", structure::OperatorNode::Type::Or},

			{"(", structure::OperatorNode::Type::Call},
			{"[", structure::OperatorNode::Type::Array}
		};

		auto position1 = position();
		save();
		skipEmpty();
		push();

		for (auto op : operators) {
			if (get(op.first.size()) == op.first) {
				skip(op.first.size());

				switch (op.second) {
				case structure::OperatorNode::Type::Call: {
					auto node = new structure::OperatorNode();
					node->type = op.second;
					parentNode->add(node);

					parseArguments(node);

					expected(')');
					break;
				}
				case structure::OperatorNode::Type::Array: {
					auto node = new structure::OperatorNode();
					node->type = op.second;
					parentNode->add(node);

					parseArguments(node);

					expected(']');
					break;
				}
				default: {
					auto node = new structure::OperatorNode();
					node->type = op.second;
					parentNode->add(node);
				}
				}

				return;
			}
		}

		auto position2 = position();

		if ((position1 != position2) && (regex::match("^[ \t]*$", configuration.code->substr(position1, position2 - position1)))) {
			auto node = new structure::OperatorNode();
			node->type = structure::OperatorNode::Type::Whitespace;
			parentNode->add(node);

			parseArguments(node);
		}

		cancel();
	}

	void OperatorParser::parseArguments(structure::OperatorNode *node) {
		while (true) {
			if (!parse<ExpressionParser>(node->expression, true)) {
				error("Expression expected.");
				cancelFatal();
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
