#pragma once

#include "parser.h"

namespace yaplc { namespace parser {
	class NumberExpressionParser : public Parser<structure::Node **> {
	protected:
		virtual void handle(structure::Node **node);
	};
} }
