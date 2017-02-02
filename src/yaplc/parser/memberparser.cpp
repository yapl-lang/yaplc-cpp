#include "memberparser.h"
#include "typeparser.h"
#include "variablememberparser.h"
#include "methodmemberparser.h"

namespace yaplc { namespace parser {
	void MemberParser::handle(structure::Childable *parentNode) {
		skipEmpty();
		push();
		
		if (end()) {
			cancel();
		}
		
		false
		|| parse<TypeParser>(parentNode)
		|| parse<VariableMemberParser>(parentNode)
		|| parse<MethodMemberParser>(parentNode)
		|| cancel();
	}
} }
