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
			base->type = "yapl.Object";
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

	public:
		virtual void load(const binstream::stream &stream) {
			TypeNode::load(stream);

			NodeFactory::loadNode(stream, base);

			for (auto interface : interfaces) {
				delete interface;
			}
			interfaces.clear();

			unsigned long count;
			stream.get(count);
			interfaces.reserve(count);

			while (count-- != 0) {
				auto interface = new TypeNameNode();
				NodeFactory::loadNode(stream, interface);
				interfaces.push_back(interface);
			}
		}

		virtual void save(binstream::stream &stream) const {
			TypeNode::save(stream);

			NodeFactory::saveNode(stream, base);

			stream.put((unsigned long)interfaces.size());
			for (auto interface : interfaces) {
				NodeFactory::saveNode(stream, interface);
			}
		}
	};
} }
