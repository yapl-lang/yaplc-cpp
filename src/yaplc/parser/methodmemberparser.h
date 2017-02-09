#pragma once

#include "parser.h"
#include "yaplc/structure/membernode.h"

namespace yaplc { namespace parser {
	class MethodMemberParser : public Parser<structure::MemberNode *, bool> {
	protected:
		virtual void handle(structure::MemberNode *parentNode, bool withoutBody);
	};
} }
