#pragma once

#include "parser.h"
#include "yaplc/structure/membernode.h"

namespace yaplc { namespace parser {
	class MemberHeadParser : public Parser<structure::Childable *, structure::MemberNode **> {
	protected:
		virtual void handle(structure::Childable *parentNode, structure::MemberNode **memberNode);
	};
} }
