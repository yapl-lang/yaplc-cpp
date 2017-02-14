#pragma once

#include "Listable.h"
#include "TypeNameNode.h"

namespace yaplc { namespace structure {
	class ClassNode : public Listable {
	public:
		TypeNameNode *name;
		TypeNameNode *base;
		std::vector<TypeNameNode *> interfaces;

	public:
		inline ClassNode() :
			Listable(),
			name(new TypeNameNode()),
			base(new TypeNameNode()),
			interfaces() {
			base->type = "Object";
		}
		
		virtual ~ClassNode() {
			delete base;
			delete name;
			for (auto interface : interfaces) {
				delete interface;
			}
		}
		
		NODE_PROPS(
			NODE_PROP_AUTO(name)
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
