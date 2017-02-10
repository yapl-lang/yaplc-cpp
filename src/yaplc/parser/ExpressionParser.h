#pragma once

#include "Parser.h"
#include "yaplc/structure/ValueNode.h"

namespace yaplc { namespace parser {
	class ExpressionParser : public Parser<structure::ValueNode *, bool> {
	protected:
		virtual void handle(structure::ValueNode *node, bool dynamic = true);
	};
} }
