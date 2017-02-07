#pragma once

#include "container.h"
#include <string>
#include <vector>

namespace yaplc { namespace structure {
	class MemberNode : public Container {
	public:
		enum class Visibility {Public, Protected, Private};
		enum class Staticality {Dynamic, Static};

	public:
		Visibility visibility;
		Staticality staticality;
		std::vector<std::pair<std::string, std::pair<unsigned long, unsigned long>>> modifiers;

		std::string type;


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
					stream << *it;
					++it;

					for (; it != end; ++it) {
						stream << ", " << *it;
					}
				}
			});

			NODE_PROP(type, stream << type;)
		)
	};
} }
