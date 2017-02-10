#pragma once

#include "Parser.h"
#include "yaplc/structure/TypeNameNode.h"

namespace yaplc { namespace parser {
	class TypeNameParser : public Parser<structure::TypeNameNode **> {
	protected:
		virtual void handle(structure::TypeNameNode **typeNameNode);
	};
} }
