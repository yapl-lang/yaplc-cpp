#include "memberparser.h"
#include "typeparser.h"
#include "memberheadparser.h"
#include "variablememberparser.h"
#include "methodmemberparser.h"

namespace yaplc { namespace parser {
	void MemberParser::handle(structure::MemberNode **node) {
		skipEmpty();
		push();
		
		if (end()) {
			cancel();
		}
		
		auto memberNode = new structure::MemberNode();
		*node = memberNode;
		if (!parse<MemberHeadParser>(memberNode)) {
			delete *node;
			*node = nullptr;
			cancel();
		}

		/*parse<TypeParser>(node)
		|| parse<VariableMemberParser>(node)
		|| parse<MethodMemberParser>(node)
		|| */cancel();
	}
} }
