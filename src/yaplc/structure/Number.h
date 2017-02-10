#pragma once

#include "listable.h"

namespace yaplc { namespace structure {
	class Number : public Listable {
	public:
		enum class Type {Add, Subtract, Multiply, Devide};
		
	public:
		Type type;
		
	};
} }
