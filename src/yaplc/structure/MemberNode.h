#pragma once

#include "Container.h"
#include "TypeNameNode.h"
#include <string>
#include <vector>
#include <tuple>

namespace yaplc { namespace structure {
	class MemberNode : public Container {
	public:
		enum class Visibility {Public, Protected, Private};
		enum class Staticality {Dynamic, Static};

	public:
		Visibility visibility;
		Staticality staticality;
		std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers;

		TypeNameNode *type;

	public:
		inline MemberNode() :
			visibility(Visibility::Public),
			staticality(Staticality::Dynamic),
			modifiers(),
			type(nullptr) {
			
		}
		
		virtual ~MemberNode() {
			delete type;
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

			NODE_PROP(staticality, switch (staticality) {
			case Staticality::Dynamic:
				stream << "dynamic";
				break;
			case Staticality::Static:
				stream << "static";
				break;
			})

			NODE_PROP(modifiers, {
				stream << "[";
				auto it = modifiers.begin();
				auto end = modifiers.end();

				if (it != end) {
					stream << std::get<0>(*it);
					++it;

					for (; it != end; ++it) {
						stream << ", " << std::get<0>(*it);
					}
				}
				stream << "]";
			});

			NODE_PROP_AUTO(type)
		)

	public:
		virtual void load(const binstream::stream &stream) {
			Container::load(stream);

			stream.get((unsigned char &)visibility);
			stream.get((unsigned char &)staticality);

			unsigned long count;
			stream.get(count);
			modifiers.clear();
			modifiers.reserve(count);
			while (count-- != 0) {
				std::string modifier;
				unsigned long begin, end;
				stream.getString(modifier);
				stream.get(begin);
				stream.get(end);

				modifiers.push_back(std::make_tuple(modifier, begin, end));
			}

			delete type;
			type = (TypeNameNode *)NodeFactory::loadNode(stream);
		}

		virtual void save(binstream::stream &stream) const {
			Container::save(stream);

			stream.put((unsigned char)visibility);
			stream.put((unsigned char)staticality);

			stream.put((unsigned long)modifiers.size());
			for (auto modifier : modifiers) {
				stream.putString(std::get<0>(modifier));
				stream.put(std::get<1>(modifier));
				stream.put(std::get<2>(modifier));
			}

			NodeFactory::saveNode(stream, type);
		}
	};
} }
