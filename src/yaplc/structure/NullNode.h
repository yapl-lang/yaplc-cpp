#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class NullNode : public Node {
	public:
		virtual void load(const binstream &stream) {
			Node::load(stream);
		}

		virtual void save(binstream &stream) const {
			Node::save(stream);
		}
	};
} }
