#pragma once

#include "childable.h"

namespace yaplc { namespace structure {
	class ClassNode : public Childable {
	public:
		enum class Visibility { Private, Protected, Public };
		
	public:
		Visibility visibility;
		std::string base;
		std::vector<std::string> interfaces;
		
	public:
		inline ClassNode(const std::string &name) :
			Childable(name),
			visibility(Visibility::Public),
			base("Object"),
			interfaces() {
			
		}
		
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
			
			NODE_PROP(base, stream << base;)
			NODE_PROP(interfaces, {
				auto i = interfaces.begin();
				auto end = interfaces.end();
				
				stream << "[";
				if (i != end) {
					stream << *(i++);
					
					for (; i < end; ++i) {
						stream << ", " << *i;
					}
				}
				stream << "]";
			})
		)
	};
} }
