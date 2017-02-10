#pragma once

#include "Parser.h"
#include "yaplc/structure/ValueNode.h"

namespace yaplc { namespace parser {
	class NumberExpressionParser : public Parser<structure::ValueNode *> {
	protected:
		virtual void handle(structure::ValueNode *node);
	};
} }
