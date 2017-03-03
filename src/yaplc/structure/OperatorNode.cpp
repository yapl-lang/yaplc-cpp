#include "OperatorNode.h"

namespace yaplc { namespace structure {
	std::vector<std::pair<std::string, OperatorNode::Type>> OperatorNode::Operators = {
		{"==", OperatorNode::Type::Equality},
		{"!=", OperatorNode::Type::Inequality},

		{"+=", OperatorNode::Type::AddSet},
		{"-=", OperatorNode::Type::SubtractSet},
		{"*=", OperatorNode::Type::MultiplySet},
		{"/=", OperatorNode::Type::DivideSet},

		{"++", OperatorNode::Type::Increment},
		{"--", OperatorNode::Type::Decrement},
		{"=", OperatorNode::Type::Set},

		{"+", OperatorNode::Type::Add},
		{"-", OperatorNode::Type::Subtract},
		{"*", OperatorNode::Type::Multiply},
		{"/", OperatorNode::Type::Divide},

		{"%", OperatorNode::Type::Module},

		{">=", OperatorNode::Type::MoreEqual},
		{"<=", OperatorNode::Type::LessEqual},

		{">", OperatorNode::Type::More},
		{"<", OperatorNode::Type::Less},

		{"!", OperatorNode::Type::Not},
		{"&&", OperatorNode::Type::And},
		{"||", OperatorNode::Type::Or},

		{"(", OperatorNode::Type::Call},
		{"[", OperatorNode::Type::Array}
	};
} }
