#pragma once

#include "Childable.h"
#include "TypeNameNode.h"

namespace yaplc { namespace structure {
	class TypeNode : public Listable {
	public:
		enum class Visibility {Private, Protected, Public};

	public:
		Visibility visibility;
		TypeNameNode *name;

	public:
		inline TypeNode() :
			Listable(),
			visibility(Visibility::Public),
			name(nullptr) {

		}

		virtual ~TypeNode() {
			delete name;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(name)
		)

	public:
		virtual void load(const binstream &stream) {
			Listable::load(stream);

			stream.get((unsigned char &)visibility);
			name = (TypeNameNode *)NodeFactory::loadNode(stream);
		}

		virtual void save(binstream &stream) const {
			Listable::save(stream);

			stream.put((unsigned char)visibility);
			NodeFactory::saveNode(stream, name);
		}
	};
} }
