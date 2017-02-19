#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class ImportNode : public Node {
	public:
		bool isStatic;
		std::string target;

		NODE_PROPS(
			NODE_PROP(isStatic, stream << ((isStatic) ? ("true") : ("false")))
			NODE_PROP(target, stream << target)
		)

	public:
		virtual void load(const binstream::stream &stream) {
			Node::load(stream);

			stream.get(isStatic);
			stream.getString(target);
		}

		virtual void save(binstream::stream &stream) const {
			Node::save(stream);

			stream.put(isStatic);
			stream.putString(target);
		}
	};
} }
