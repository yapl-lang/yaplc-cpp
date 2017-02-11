#pragma once

#include <yaplc/structure/OperatorNode.h>
#include "Parser.h"
#include "yaplc/structure/Listable.h"

namespace yaplc { namespace parser {
	class OperatorParser : public Parser<structure::Listable *> {
	protected:
		virtual void handle(structure::Listable *parentNode);

		void parseArguments(structure::OperatorNode *node);
	};
} }
