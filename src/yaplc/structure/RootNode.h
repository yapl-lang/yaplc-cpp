#pragma once

#include "Listable.h"

namespace yaplc { namespace structure {
	class RootNode : public Listable {
	public:
		virtual void load(const binstream &stream) {
			Listable::load(stream);
		}

		virtual void save(binstream &stream) const {
			Listable::save(stream);
		}
	};
} }
