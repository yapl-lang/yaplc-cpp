#pragma once

#include "parser.h"
#include "yaplc/structure/valuenode.h"

namespace yaplc { namespace parser {
	class ExpressionParser : public Parser<structure::ValueNode *, bool> {
	protected:
		virtual void handle(structure::ValueNode *node, bool dynamic = true);
	};
} }
