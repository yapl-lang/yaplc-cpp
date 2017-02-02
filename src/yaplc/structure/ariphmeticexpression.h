#pragma once

#include "listable.h"

namespace yaplc { namespace structure {
	class AriphmeticExpression : public Listable {
	public:
		std::string value;
		
		NODE_PROPS(
			NODE_PROP(value, stream << value;)
		)
	};
} }
