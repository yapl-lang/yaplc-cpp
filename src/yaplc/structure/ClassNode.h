#pragma once

#include "Childable.h"
#include "TypeNameNode.h"

namespace yaplc { namespace structure {
	class ClassNode : public Childable {
	public:
		enum class Visibility { Private, Protected, Public };
		
	public:
		Visibility visibility;
		TypeNameNode *base;
		std::vector<TypeNameNode *> interfaces;

	public:
		inline ClassNode(const std::string &name) :
			Childable(name),
			visibility(Visibility::Public),
			base(new TypeNameNode()),
			interfaces() {
			base->type = "Object";
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
			
			NODE_PROP_AUTO(base)
			NODE_PROP(interfaces, {
				auto i = interfaces.begin();
				auto end = interfaces.end();
				
				stream << "[";
				if (i != end) {
					(*i)->show(stream, indent + 1);
					
					for (++i; i < end; ++i) {
						stream << ", ";
						(*i)->show(stream, indent + 1);
					}
				}
				stream << "]";
			})
		)
	};
} }
