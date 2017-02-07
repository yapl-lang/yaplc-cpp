#pragma once

#include "parser.h"
#include "yaplc/structure/childable.h"

namespace yaplc { namespace parser {
	class TypeParser : public Parser<structure::Childable *> {
	protected:
		virtual void handle(structure::Childable *parentNode);
	};
} }
