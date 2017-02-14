#pragma once

#include "Node.h"
#include <vector>
#include <string>

namespace yaplc { namespace structure {
	class TypeNameNode : public Node {
	public:
		std::string type;
		std::vector<TypeNameNode *> templateArguments;

	public:
		inline TypeNameNode() :
			Node(),
			type(),
			templateArguments() {
			
		}
		
		virtual ~TypeNameNode() {
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
