#pragma once

#include "Listable.h"

namespace yaplc { namespace structure {
	class ExpressionNode : public Listable {
	public:
		virtual void load(const binstream::stream &stream) {
			Listable::load(stream);
		}

		virtual void save(binstream::stream &stream) const {
			Listable::save(stream);
		}
	};
} }
