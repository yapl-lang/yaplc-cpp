#pragma once

#include "Parser.h"
#include "yaplc/structure/Childable.h"

namespace yaplc { namespace parser {
	class MemberParser : public Parser<structure::Listable *> {
	protected:
		virtual void handle(structure::Listable *parentNode);
	};
} }
