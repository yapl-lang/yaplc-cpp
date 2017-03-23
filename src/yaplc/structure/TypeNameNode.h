#pragma once

#include "Node.h"
#include "ExpressionNode.h"
#include <vector>
#include <string>

namespace yaplc { namespace structure {
	class TypeNameNode : public Node {
	public:
		std::string type;
		std::vector<TypeNameNode *> templateArguments;
		std::vector<ExpressionNode *> dimens;

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

			for (auto dimen : dimens) {
				delete dimen;
			}
		}

		std::string typePath() const {
			auto type = hashName();

			unsigned long lastDot = -1;

			for (unsigned long i = 0; i < type.size(); ++i) {
				switch (type[i]) {
				case '.':
					lastDot = i;
					break;
				case '<':
				case '[':
					if (lastDot == -1) {
						return "";
					}

					return type.substr(0, lastDot - 1);
				}
			}

			if (lastDot == -1) {
				return "";
			}

			return type.substr(0, lastDot - 1);
		}

		std::string shortType() const {
			auto type = hashName();

			unsigned long lastDot = -1;

			for (unsigned long i = 0; i < type.size(); ++i) {
				switch (type[i]) {
				case '.':
					lastDot = i;
					break;
				case '<':
				case '[':
					if (lastDot == -1) {
						return type.substr(0);
					}

					return type.substr(lastDot + 1);
				}
			}

			if (lastDot == -1) {
				return type;
			}

			return type.substr(lastDot + 1);
		}

		inline std::string hashName() const {
			std::stringstream result;
			result << type;

			auto templateArgument = templateArguments.begin();
			if (templateArgument != templateArguments.end()) {
				result << "<";

				result << (*templateArgument)->hashName();
				++templateArgument;
				for (; templateArgument != templateArguments.end(); ++templateArgument) {
					result << ", " << (*templateArgument)->hashName();
				}

				result << ">";
			}

			for (auto dimen : dimens) {
				result << "[]";
			}

			return result.str();
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

			NODE_PROP(dimens, {
				auto it = dimens.begin();
				auto end = dimens.end();

				if (it != end) {
					stream << "[";
					if ((*it) != nullptr) {
						(*it)->show(stream, indent + 2);
					}
					stream << "]";

					++it;

					for (; it != end; ++it) {
						stream << "[";
						if ((*it) != nullptr) {
							(*it)->show(stream, indent + 2);
						}
						stream << "]";
					}
				}
			})
		)

	public:
		virtual void load(const binstream &stream) {
			Node::load(stream);

			stream.getString(type);

			for (auto argument : templateArguments) {
				delete argument;
			}
			templateArguments.clear();

			{
				unsigned long count;
				stream.get(count);
				templateArguments.reserve(count);

				while (count-- != 0) {
					auto node = new TypeNameNode();
					NodeFactory::loadNode(stream, node);
					templateArguments.push_back(node);
				}
			}

			{
				unsigned long count;
				stream.get(count);
				templateArguments.reserve(count);

				while (count-- != 0) {
					dimens.push_back((ExpressionNode *)NodeFactory::loadNode(stream));
				}
			}
		}

		virtual void save(binstream &stream) const {
			Node::save(stream);

			stream.putString(type);

			stream.put((unsigned long)templateArguments.size());
			for (auto argument : templateArguments) {
				NodeFactory::saveNode(stream, argument);
			}

			stream.put((unsigned long)dimens.size());
			for (auto dimen : dimens) {
				NodeFactory::saveNode(stream, dimen);
			}
		}
	};
} }
