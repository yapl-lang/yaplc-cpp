#pragma once

#include "Parser.h"
#include "yaplc/structure/MemberNode.h"

namespace yaplc { namespace parser {
	class MemberHeadParser : public Parser<structure::Childable *, structure::MemberNode **> {
	protected:
		virtual void handle(structure::Childable *parentNode, structure::MemberNode **memberNode);
	};
} }
