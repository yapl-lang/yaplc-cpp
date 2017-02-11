#pragma once

#include "Parser.h"
#include "yaplc/structure/ExpressionNode.h"

namespace yaplc { namespace parser {
	class StringExpressionParser : public Parser<structure::ExpressionNode *> {
	protected:
		virtual void handle(structure::ExpressionNode *parentNode);
		
		void parseString(structure::ExpressionNode *parentNode);
		void parseChar(structure::ExpressionNode *parentNode);
		void parseTemplateString(structure::ExpressionNode *parentNode);
	};
} }
