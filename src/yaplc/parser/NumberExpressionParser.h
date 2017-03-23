#pragma once

#include "Parser.h"
#include "yaplc/structure/ExpressionNode.h"

namespace yaplc { namespace parser {
	class NumberExpressionParser : public Parser<structure::ExpressionNode *> {
	protected:
		static std::regex NumberRegex;

		virtual void handle(structure::ExpressionNode *parentNode);
	};
} }
