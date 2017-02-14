#pragma once

#include "Parser.h"
#include "yaplc/structure/ClassNode.h"

namespace yaplc { namespace parser {
	class ClassParser : public Parser<structure::ClassNode *> {
	protected:
		virtual void handle(structure::ClassNode *classNode);
	};
} }
