#pragma once

#include "Parser.h"
#include "yaplc/structure/MemberNode.h"

namespace yaplc { namespace parser {
	class VariableMemberParser : public Parser<structure::MemberNode *> {
	protected:
		virtual void handle(structure::MemberNode *parentNode);
	};
} }
