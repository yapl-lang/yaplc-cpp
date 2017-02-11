#include "OperatorParser.h"
#include "yaplc/structure/OperatorNode.h"

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
		};

		skipEmpty();
		push();

		for (auto op : operators) {
			if (get(op.first.size()) == op.first) {
				skip(op.first.size());

				auto node = new structure::OperatorNode();
				node->type = op.second;
				parentNode->add(node);

				return;
			}
		}

		cancel();
	}
} }
