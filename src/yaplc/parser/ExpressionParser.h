#pragma once

#include "Parser.h"
#include "yaplc/structure/ExpressionNode.h"

namespace yaplc { namespace parser {
	class ExpressionParser : public Parser<structure::ExpressionNode *, bool> {
	protected:
		virtual void handle(structure::ExpressionNode *node, bool dynamic = true);
	};
} }
