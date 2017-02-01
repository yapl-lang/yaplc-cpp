#pragma once

#include "parser.h"
#include "yaplc/structure/packagenode.h"

namespace yaplc { namespace parser {
	class ClassParser : public Parser<structure::Childable *> {
	protected:
		virtual void handle(structure::Childable *packageNode);
	};
} }
