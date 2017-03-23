#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class CharNode : public Node {
	public:
		std::string value;
		
		NODE_PROPS(
			NODE_PROP(value, stream << value)
		)

	public:
		virtual void load(const binstream &stream) {
			Node::load(stream);

			stream.getString(value);
		}

		virtual void save(binstream &stream) const {
			Node::save(stream);

			stream.putString(value);
		}
	};
} }
