#pragma once

#include "Node.h"
#include "TypeNameNode.h"
#include "ExpressionNode.h"
#include <tuple>

namespace yaplc { namespace structure {
	class ArgumentsNode : public Node {
	public:
		std::vector<std::tuple<TypeNameNode *, std::string, ExpressionNode *>> arguments;

	public:
		virtual ~ArgumentsNode() {
			for (auto argument : arguments) {
				TypeNameNode *type;
				std::string name;
				ExpressionNode *value;

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
					TypeNameNode *type;
					std::string name;
					ExpressionNode *value;

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
