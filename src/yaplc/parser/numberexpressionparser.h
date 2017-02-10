#pragma once

#include "parser.h"
#include "yaplc/structure/valuenode.h"

namespace yaplc { namespace parser {
	class NumberExpressionParser : public Parser<structure::ValueNode *> {
	protected:
		virtual void handle(structure::ValueNode *node);
	};
} }
