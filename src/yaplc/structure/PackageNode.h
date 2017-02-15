#pragma once

#include "Listable.h"

namespace yaplc { namespace structure {
	class PackageNode : public Listable {
	public:
		std::string name;
		
		NODE_PROPS(
			NODE_PROP(name, stream << name)
		)
	};
} }
