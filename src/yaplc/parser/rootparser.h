#pragma once

#include "parser.h"
#include "yaplc/structure/rootnode.h"

namespace yaplc { namespace parser {
	class RootParser : public Parser<structure::RootNode *> {
	protected:
		virtual void handle(structure::RootNode *rootNode);
	};
} }
