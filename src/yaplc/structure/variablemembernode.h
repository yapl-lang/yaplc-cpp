#pragma once

#include "container.h"

namespace yaplc { namespace structure {
	class VariableMemberNode : public Container {
	public:
		enum class Visibility {Public, Protected, Private};
		enum class Staticality {Dynamic, Static};
		
	public:
		Visibility visibility;
		Staticality staticality;
		std::string type;
		std::string defaultValue;
		
		NODE_PROPS(
			NODE_PROP(visibility, switch (visibility) {
			case Visibility::Private:
				stream << "private";
				break;
			case Visibility::Protected:
				stream << "protected";
				break;
			case Visibility::Public:
				stream << "public";
				break;
			})
			
			NODE_PROP(staticality, switch (staticality) {
			case Staticality::Dynamic:
				stream << "dynamic";
				break;
			case Staticality::Static:
				stream << "static";
				break;
			})
			
			NODE_PROP(type, stream << type;)
			NODE_PROP(defaultValue, stream << defaultValue;);
		)
		
	public:
		inline VariableMemberNode() :
			visibility(Visibility::Public),
			staticality(Staticality::Dynamic) {}
	};
} }
