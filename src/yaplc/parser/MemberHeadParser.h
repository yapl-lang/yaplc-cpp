#pragma once

#include "Parser.h"
#include "yaplc/structure/MemberNode.h"

namespace yaplc { namespace parser {
	class MemberHeadParser : public Parser<structure::Listable *, structure::MemberNode **> {
	protected:
		virtual void handle(structure::Listable *parentNode, structure::MemberNode **memberNode);
	};
} }
