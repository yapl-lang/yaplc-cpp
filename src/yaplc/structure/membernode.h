#pragma once

#include "container.h"
#include "typereferencenode.h"
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

		TypeReferenceNode *type;

	public:
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
				auto it = modifiers.begin();
				auto end = modifiers.end();

				if (it != end) {
					stream << std::get<0>(*it);
					++it;

					for (; it != end; ++it) {
						stream << ", " << std::get<0>(*it);
					}
				}
			});

			NODE_PROP(type, type->show(stream, indent + 1);)
		)
	};
} }
