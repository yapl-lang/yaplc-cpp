#pragma once

#include "Parser.h"
#include "yaplc/structure/MemberNode.h"
#include <regex>

namespace yaplc { namespace parser {
	class MemberHeadParser : public Parser<structure::Listable *, structure::MemberNode **> {
	protected:
		static std::regex ModifierNameRegex;
		static std::regex MemberNameRegex;

		virtual void handle(structure::Listable *parentNode, structure::MemberNode **memberNode);
	};
} }
