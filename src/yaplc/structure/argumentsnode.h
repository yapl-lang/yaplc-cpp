#pragma once

#include "node.h"
#include "typereferencenode.h"
#include "valuenode.h"
#include <tuple>

namespace yaplc { namespace structure {
	class ArgumentsNode : public Node {
	public:
		std::vector<std::tuple<TypeReferenceNode *, std::string, ValueNode *>> arguments;

	public:
		virtual ~ArgumentsNode() {
			for (auto argument : arguments) {
				TypeReferenceNode *type;
				std::string name;
				ValueNode *value;

				std::tie(type, name, value) = argument;

				delete type;
				delete value;
			}
		}

		NODE_PROPS(
			NODE_PROP(arguments, {
				// TODO:

				/*for (auto argument : arguments) {
					TypeReferenceNode *type;
					std::string name;
					ValueNode *value;

					std::tie(type, name, value) = argument;


				}*/
			})
		)
	};
} }
