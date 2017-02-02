#pragma once

#include "parser.h"
#include "yaplc/structure/node.h"

namespace yaplc { namespace parser {
	class ExpressionParser : public Parser<structure::Node **, bool> {
	protected:
		virtual void handle(structure::Node **node, bool dynamic = true);
	};
} }
