#pragma once

#include "parser.h"
#include "yaplc/structure/typereferencenode.h"

namespace yaplc { namespace parser {
	class TypeReferenceParser : public Parser<structure::TypeReferenceNode **> {
	protected:
		virtual void handle(structure::TypeReferenceNode **typeReferenceNode);
	};
} }
