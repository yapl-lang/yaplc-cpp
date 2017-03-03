#pragma once

#include "Parser.h"
#include "yaplc/structure/MemberNode.h"

namespace yaplc { namespace parser {
	class OperatorMemberParser : public Parser<structure::MemberNode *, bool> {
	protected:
		virtual void handle(structure::MemberNode *parentNode, bool withoutBody);
	};
} }
