#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class NullNode : public Node {
	public:
		virtual void load(const binstream::stream &stream) {
			Node::load(stream);
		}

		virtual void save(binstream::stream &stream) const {
			Node::save(stream);
		}
	};
} }
