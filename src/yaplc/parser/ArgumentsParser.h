#pragma once

#include "Parser.h"
#include "yaplc/structure/ArgumentsNode.h"

namespace yaplc { namespace parser {
	class ArgumentsParser : public Parser<structure::ArgumentsNode *> {
	protected:
		virtual void handle(structure::ArgumentsNode *argumentsNode);
	};
} }
