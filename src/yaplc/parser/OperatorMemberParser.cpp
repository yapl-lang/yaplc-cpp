#include "OperatorMemberParser.h"
#include "yaplc/structure/OperatorMemberNode.h"

namespace yaplc { namespace parser {
	void OperatorMemberParser::handle(structure::MemberNode *parentNode, bool withoutBody) {
		if (parentNode->getName() != "operator") {
			cancel();
		}
		
		cancel();
	}
} }
