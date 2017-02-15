#include "TypeExpressionParser.h"
#include "TypeNameParser.h"
#include "yaplc/structure/TypeExpressionNode.h"
#include "OperatorParser.h"

namespace yaplc { namespace parser {
	void TypeExpressionParser::handle(structure::ExpressionNode *parentNode) {
		structure::TypeNameNode *typeNameNode = nullptr;

		begin();
		if (!parse<TypeNameParser>(&typeNameNode)) {
			cancel();
		}

		auto node = new structure::TypeExpressionNode();
		node->type = typeNameNode;
		parentNode->add(node);
		end(node);

		structure::TypeNameNode *typeNameNode2 = nullptr;

		save();
		while ((skipChar('.')) && (parse<TypeNameParser>(&typeNameNode2))) {
			auto node2 = new structure::TypeExpressionNode();
			node->child = node2;
			node = node2;
			node->type = typeNameNode2;

			norestore();
			save();
		}
		restore();
	}
} }
