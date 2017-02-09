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
				stream << "[";

				bool first = true;

				for (auto argument : arguments) {
					TypeReferenceNode *type;
					std::string name;
					ValueNode *value;

					std::tie(type, name, value) = argument;

					if (!first) {
						stream << ", ";
					}

					stream << name << " => ";
					type->show(stream, indent + 2);

					if (value != nullptr) {
						stream << " = ";
						value->show(stream, indent + 2);
					}

					first = false;
				}
				stream << "]";
			})
		)
	};
} }
