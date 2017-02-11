#include "NullExpressionParser.h"
#include "yaplc/structure/NullNode.h"

namespace yaplc { namespace parser {
	void NullExpressionParser::handle(structure::ExpressionNode *parentNode) {
		cancelIfEnd();
		
		skipOrCancel("null");
		
		parentNode->add(new structure::NullNode());
	}
} }
