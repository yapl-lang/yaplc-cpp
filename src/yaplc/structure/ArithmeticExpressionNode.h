#pragma once

#include "Listable.h"

namespace yaplc { namespace structure {
	class ArithmeticExpression : public Listable {
	public:
		std::string value;
		
		NODE_PROPS(
			NODE_PROP(value, stream << value;)
		)
	};
} }
