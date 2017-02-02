#pragma once

#include "parser.h"

namespace yaplc { namespace parser {
	class MethodMemberParser : public Parser<structure::Childable *> {
	protected:
		virtual void handle(structure::Childable *parentNode);
	};
} }
