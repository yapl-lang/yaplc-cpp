#include "memberheadparser.h"
#include "regex/regex.h"
#include "yaplc/util/map.h"

namespace yaplc { namespace parser {
	void MemberHeadParser::handle(structure::Childable *parentNode, structure::MemberNode **memberNode) {
		std::vector<std::pair<std::string, std::pair<unsigned long, unsigned long>>> modifiers;

		if (get("[A-Za-z0-9_]*", modifiers) < 2) {
			cancel();
		}

		auto it = modifiers.end();
		auto name = (*(--it));
		auto type = (*(--it));
		modifiers.resize(modifiers.size() - 2);

		for (auto modifier : modifiers) {
			if (!regex::match("^[A-Za-z_][A-Za-z0-9_]*$", modifier.first)) {
				error("Invalid modifier name.", modifier.second.first, modifier.second.second);
			}
		}

		if (!regex::match("^[A-Za-z_][A-Za-z0-9_]*$", type.first)) {
			error("Invalid type name.", type.second.first, type.second.second);
		}

		if (!regex::match("^[A-Za-z_][A-Za-z0-9_]*$", name.first)) {
			error("Invalid member name.", name.second.first, name.second.second);
		}

		*memberNode = new structure::MemberNode();
		parentNode->add(*memberNode);
		(*memberNode)->setName(name.first);
		(*memberNode)->type = type.first;

		{
			std::map<std::string, std::string> outModifiers;
			std::vector<std::pair<std::string, std::pair<unsigned long, unsigned long>>> otherModifiers;
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
