#pragma once

#include "parser.h"
#include "yaplc/structure/node.h"

namespace yaplc { namespace parser {
	class MethodMemberParser : public Parser<structure::Node **> {
	protected:
		virtual void handle(structure::Node **node);
	};
} }
