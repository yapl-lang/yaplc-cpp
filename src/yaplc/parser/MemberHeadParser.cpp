#include "MemberHeadParser.h"
#include "regex/regex.h"
#include "yaplc/util/map.h"
#include "TypeNameParser.h"
#include "yaplc/structure/TypeNameNode.h"
#include <tuple>

namespace yaplc { namespace parser {
	void MemberHeadParser::handle(structure::Listable *parentNode, structure::MemberNode **memberNode) {
		std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers;

		// Hardcode
		std::tuple<std::string, unsigned long, unsigned long> name;
		structure::TypeNameNode *type;

		{
			std::vector<std::tuple<structure::TypeNameNode *, unsigned long, unsigned long>> entries;

			while (true) {
				structure::TypeNameNode *typeNameNode;

				skipEmpty();
				unsigned long start = position();
				if (!parse<TypeNameParser>(&typeNameNode)) {
					break;
				}

				entries.push_back(std::make_tuple(typeNameNode, start, position() - 1));
			}

			if (entries.size() < 2) {
				for (auto entry : entries) {
					delete std::get<0>(entry);
				}

				cancel();
			}

			auto nameEntry = *(--entries.end());
			entries.pop_back();
			name = std::make_tuple(std::get<0>(nameEntry)->type, std::get<1>(nameEntry), std::get<2>(nameEntry) - 1);
			delete std::get<0>(nameEntry);

			type = std::get<0>(*(--entries.end()));
			entries.pop_back();

			for (auto entry : entries) {
				structure::TypeNameNode *typeNode;
				unsigned long begin, end;

				std::tie(typeNode, begin, end) = entry;

				auto modifier = typeNode->type;
				delete typeNode;

				if (regex::match("^[A-Za-z_][A-Za-z0-9_]*$", modifier)) {
					modifiers.push_back(std::make_tuple(modifier, begin, end - 1));
				} else {
					error("Invalid modifier name.", begin, end - 1);
				}
			}
		}

		if (!regex::match("^[A-Za-z_][A-Za-z0-9_]*$", std::get<0>(name))) {
			error("Invalid member name.", std::get<1>(name), std::get<2>(name));
		}

		*memberNode = new structure::MemberNode();
		parentNode->add(*memberNode);
		(*memberNode)->setName(std::get<0>(name));
		(*memberNode)->type = type;

		{
			std::map<std::string, std::string> outModifiers;
			std::vector<std::tuple<std::string, unsigned long, unsigned long>> otherModifiers;
			groupModifiers({
				{"visibility", {"public", "protected", "private"}},
				{"staticality", {"~dynamic", "static"}}
			}, modifiers, outModifiers, otherModifiers);

			{
				auto visibilityModifier = outModifiers["visibility"];

				if (visibilityModifier == "public") {
					(*memberNode)->visibility = structure::MemberNode::Visibility::Public;
				} else if (visibilityModifier == "protected") {
					(*memberNode)->visibility = structure::MemberNode::Visibility::Protected;
				} else if (visibilityModifier == "private") {
					(*memberNode)->visibility = structure::MemberNode::Visibility::Private;
				}
			}

			{
				auto staticalityModifier = outModifiers["staticality"];

				if (staticalityModifier == "dynamic") {
					(*memberNode)->staticality = structure::MemberNode::Staticality::Dynamic;
				} else if (staticalityModifier == "static") {
					(*memberNode)->staticality = structure::MemberNode::Staticality::Static;
				}
			}

			(*memberNode)->modifiers = otherModifiers;
		}
	}
} }
