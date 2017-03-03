#include "OperatorNode.h"

namespace yaplc { namespace structure {
	std::vector<std::pair<std::string, OperatorNode::Type>> OperatorNode::Operators = {
		{"==", OperatorNode::Type::Equality},
		{"equals", OperatorNode::Type::Equality},
		{"!=", OperatorNode::Type::Inequality},
		{"dequals", OperatorNode::Type::Inequality},

		{"+=", OperatorNode::Type::AddSet},
		{"addme", OperatorNode::Type::AddSet},
		{"-=", OperatorNode::Type::SubtractSet},
		{"subtractme", OperatorNode::Type::SubtractSet},
		{"*=", OperatorNode::Type::MultiplySet},
		{"multiplyme", OperatorNode::Type::MultiplySet},
		{"/=", OperatorNode::Type::DivideSet},
		{"devideme", OperatorNode::Type::DivideSet},

		{"++", OperatorNode::Type::Increment},
		{"increment", OperatorNode::Type::Increment},
		{"--", OperatorNode::Type::Decrement},
		{"decrement", OperatorNode::Type::Decrement},
		{"=", OperatorNode::Type::Set},
		{"set", OperatorNode::Type::Set},

		{"+", OperatorNode::Type::Add},
		{"add", OperatorNode::Type::Add},
		{"-", OperatorNode::Type::Subtract},
		{"subtract", OperatorNode::Type::Subtract},
		{"*", OperatorNode::Type::Multiply},
		{"multiply", OperatorNode::Type::Multiply},
		{"/", OperatorNode::Type::Divide},
		{"devide", OperatorNode::Type::Divide},

		{"%", OperatorNode::Type::Module},
		{"module", OperatorNode::Type::Module},

		{">=", OperatorNode::Type::MoreEqual},
		{"moreeq", OperatorNode::Type::MoreEqual},
		{"<=", OperatorNode::Type::LessEqual},
		{"lesseq", OperatorNode::Type::LessEqual},

		{">", OperatorNode::Type::More},
		{"more", OperatorNode::Type::More},
		{"<", OperatorNode::Type::Less},
		{"less", OperatorNode::Type::Less},

		{"!", OperatorNode::Type::Not},
		{"not", OperatorNode::Type::Not},
		{"&&", OperatorNode::Type::And},
		{"and", OperatorNode::Type::And},
		{"||", OperatorNode::Type::Or},
		{"or", OperatorNode::Type::Or},

		{"(", OperatorNode::Type::Call},
		{"[", OperatorNode::Type::Array},

		{"whitespace", OperatorNode::Type::Whitespace},
	};
} }
