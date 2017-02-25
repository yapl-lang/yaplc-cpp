#pragma once

#include "Parser.h"
#include "yaplc/structure/Node.h"

namespace yaplc { namespace parser {
	class SpecialParser : public Parser<structure::Node *> {
	protected:
		virtual void handle(structure::Node *parentNode);
	};
} }
