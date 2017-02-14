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
			name(new TypeNameNode()) {

		}

		virtual ~TypeNode() {
			delete name;
		}

		NODE_PROPS(
			NODE_PROP_AUTO(name)
		)
	};
} }
