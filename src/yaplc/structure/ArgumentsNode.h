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

	public:
		virtual void load(const binstream &stream) {
			Node::load(stream);

			unsigned long count;
			stream.get(count);

			for (auto argument : arguments) {
				delete std::get<0>(argument);
				delete std::get<2>(argument);
			}
			arguments.clear();
			arguments.reserve(count);

			while (count-- != 0) {
				auto type = new TypeNameNode();
				std::string name;
				ExpressionNode *value = nullptr;

				NodeFactory::loadNode(stream, type);
				stream.getString(name);
				value = (ExpressionNode *)NodeFactory::loadNode(stream);

				arguments.push_back(std::make_tuple(type, name, value));
			}
		}

		virtual void save(binstream &stream) const {
			Node::save(stream);

			stream.put((unsigned long)arguments.size());

			for (auto argument : arguments) {
				TypeNameNode *type;
				std::string name;
				ExpressionNode *value;

				std::tie(type, name, value) = argument;

				NodeFactory::saveNode(stream, type);
				stream.putString(name);
				NodeFactory::saveNode(stream, value);
			}
		}
	};
} }
