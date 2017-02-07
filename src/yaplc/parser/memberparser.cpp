#include "memberparser.h"
#include "typeparser.h"
#include "memberheadparser.h"
#include "variablememberparser.h"
#include "methodmemberparser.h"

namespace yaplc { namespace parser {
	void MemberParser::handle(structure::Childable *parentNode) {
		skipEmpty();
		push();
		
		if (end()) {
			cancel();
		}

		if (parse<TypeParser>(parentNode)) {
			return;
		}

		structure::MemberNode *memberNode;
		if (!parse<MemberHeadParser>(parentNode, &memberNode)) {
			cancel();
		}


		parse<MethodMemberParser>(memberNode)
		//|| parse<VariableMemberParser>(node)
		|| cancel();
	}
} }
