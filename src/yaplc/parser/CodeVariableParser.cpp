#include "CodeVariableParser.h"
#include "TypeNameParser.h"
#include "ExpressionParser.h"
#include "yaplc/structure/DeclareVariableNode.h"

namespace yaplc { namespace parser {
	void CodeVariableParser::handle(structure::ExpressionNode *parentNode) {
		structure::TypeNameNode *typeNameNode;

		if (!parse<TypeNameParser>(&typeNameNode)) {
			cancel();
		}

		auto node = new structure::DeclareVariableNode();
		parentNode->add(node);
		node->type = typeNameNode;

		if (!getWord(node->name)) {
			cancel();
		}

		skipEmpty();

		if (skipChar('=')) {
			if (!parse<ExpressionParser>(node->value = new structure::ExpressionNode(), true)) {
				error("Value expected.");
			}
		}
	}
} }
