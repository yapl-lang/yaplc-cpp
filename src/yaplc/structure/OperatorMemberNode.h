#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class OperatorMemberNode : public Node {
	public:

	public:
		virtual void load(const binstream::stream &stream) {
			Node::load(stream);
		}

		virtual void save(binstream::stream &stream) const {
			Node::save(stream);
		}
	};
} }
