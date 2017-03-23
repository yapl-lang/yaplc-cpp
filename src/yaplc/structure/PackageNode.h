#pragma once

#include "Listable.h"

namespace yaplc { namespace structure {
	class PackageNode : public Listable {
	public:
		std::string name;
		
		NODE_PROPS(
			NODE_PROP(name, stream << name)
		)

	public:
		virtual void load(const binstream &stream) {
			Listable::load(stream);

			stream.getString(name);
		}

		virtual void save(binstream &stream) const {
			Listable::save(stream);

			stream.putString(name);
		}
	};
} }
