#pragma once

#include "parser.h"
#include "yaplc/structure/membernode.h"

namespace yaplc { namespace parser {
	class MemberHeadParser : public Parser<structure::MemberNode *> {
	protected:
		virtual void handle(structure::MemberNode *memberNode);
	};
} }
