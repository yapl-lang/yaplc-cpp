#pragma once

#include "node.h"
#include <vector>
#include <string>

namespace yaplc { namespace structure {
	class TypeReferenceNode : public Node {
	public:
		std::string type;
		std::vector<TypeReferenceNode *> templateArguments;

	public:
		virtual ~TypeReferenceNode() {
			for (auto templateArgument : templateArguments) {
				delete templateArgument;
			}
		}

		NODE_PROPS(
			NODE_PROP(type, stream << type;)

			NODE_PROP(templateArguments, {
				stream << "<";

				auto it = templateArguments.begin();
				auto end = templateArguments.end();

				if (it != end) {
					stream << std::endl;

					util::leftpad(stream, indent + 2);
					(*it)->show(stream, indent + 2);
					++it;

					for (; it != end; ++it) {
						stream << ", ";

						(*it)->show(stream, indent + 2);
					}
				}

				stream << ">";
			})
		)
	};
} }
