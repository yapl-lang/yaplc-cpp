#pragma once

#include "Parser.h"
#include "yaplc/structure/Childable.h"

namespace yaplc { namespace parser {
	class TypeParser : public Parser<structure::Listable *> {
	protected:
		virtual void handle(structure::Listable *parentNode);
	};
} }
