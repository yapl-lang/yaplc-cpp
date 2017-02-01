#pragma once

#include "listable.h"

namespace yaplc { namespace structure {
	class Number : public Listable {
	public:
		enum class Type {Add, Substract, Multiply, Delim};
		
	public:
		Type type;
		
	};
} }
