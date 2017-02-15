#pragma once

#include "TypeNode.h"
#include "TypeNameNode.h"

namespace yaplc { namespace structure {
	class ClassNode : public TypeNode {
	public:
		TypeNameNode *base;
		std::vector<TypeNameNode *> interfaces;

	public:
		inline ClassNode() :
			TypeNode(),
			base(new TypeNameNode()),
			interfaces() {
			base->type = "std.Object";
		}
		
		virtual ~ClassNode() {
			delete base;
			for (auto interface : interfaces) {
				delete interface;
			}
		}
		
		NODE_PROPS_PARENT(
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
		, TypeNode)
	};
} }
