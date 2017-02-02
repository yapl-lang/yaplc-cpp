#include "memberparser.h"
#include "typeparser.h"
#include "variablememberparser.h"
#include "methodmemberparser.h"

namespace yaplc { namespace parser {
	void MemberParser::handle(structure::Node **node) {
		skipEmpty();
		push();
		
		if (end()) {
			cancel();
		}

		parse<TypeParser>(node)
		|| parse<VariableMemberParser>(node)
		|| parse<MethodMemberParser>(node)
		|| cancel();
	}
} }
