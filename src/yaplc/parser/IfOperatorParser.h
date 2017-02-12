#pragma once

#include "Parser.h"
#include "yaplc/structure/ExpressionNode.h"

namespace yaplc { namespace parser {
	class IfOperatorParser : public Parser<structure::ExpressionNode *> {
	protected:
		virtual void handle(structure::ExpressionNode *parentNode);
	};
} }
