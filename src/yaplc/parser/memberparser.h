#pragma once

#include "parser.h"
#include "yaplc/structure/membernode.h"

namespace yaplc { namespace parser {
	class MemberParser : public Parser<structure::MemberNode **> {
	protected:
		virtual void handle(structure::MemberNode **node);
	};
} }
