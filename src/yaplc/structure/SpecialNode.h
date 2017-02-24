#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class SpecialNode : public Node {
	public:
		std::string data;

	public:
		virtual void load(const binstream::stream &stream) {
			Node::load(stream);

			stream.getString(data);
		}

		virtual void save(binstream::stream &stream) const {
			Node::save(stream);

			stream.putString(data);
		}
	};
} }
