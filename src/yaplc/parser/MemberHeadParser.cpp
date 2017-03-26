#include "MemberHeadParser.h"
#include "yaplc/util/map.h"
#include "TypeNameParser.h"
#include "yaplc/structure/TypeNameNode.h"
#include <tuple>

namespace yaplc { namespace parser {
	std::regex MemberHeadParser::ModifierNameRegex{"^[A-Za-z_][A-Za-z0-9_]*$"};
	std::regex MemberHeadParser::MemberNameRegex{"^[A-Za-z\\$_][A-Za-z0-9\\$_]*$"};

	void MemberHeadParser::handle(structure::Listable *parentNode, structure::MemberNode **memberNode) {
		skipEmpty();
		begin();

		std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers;

		// Hardcode
		std::tuple<std::string, unsigned long, unsigned long> name;
		structure::TypeNameNode *type = nullptr;

		{
			std::vector<std::tuple<structure::TypeNameNode *, unsigned long, unsigned long>> entries;

			while (true) {
				save();

				structure::TypeNameNode *typeNameNode = nullptr;

				skipEmpty();
				unsigned long start = position();
				if (!parse<TypeNameParser>(&typeNameNode)) {
					break;
				}

				if (typeNameNode->type == "operator") {
					restore();

					start = position();
					std::string nameBuffer;
					if (!getWord(nameBuffer)) {
						error("Wrong context.");
						cancelFatal();
					}

					typeNameNode->type = nameBuffer;
					typeNameNode->templateArguments.clear();
					typeNameNode->dimens.clear();
					entries.push_back(std::make_tuple(typeNameNode, start, position() - 1));
					break;
				}

				norestore();
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
			name = std::make_tuple(std::get<0>(nameEntry)->type, std::get<1>(nameEntry), std::get<2>(nameEntry));
			delete std::get<0>(nameEntry);

			type = std::get<0>(*(--entries.end()));
			entries.pop_back();

			for (auto entry : entries) {
				structure::TypeNameNode *typeNode;
				unsigned long begin, end;

				std::tie(typeNode, begin, end) = entry;

				auto modifier = typeNode->type;
				delete typeNode;

				if (std::regex_match(modifier, ModifierNameRegex)) {
					modifiers.push_back(std::make_tuple(modifier, begin, end));
				} else {
					error("Invalid modifier name.", begin, end - 1);
				}
			}
		}

		if (!std::regex_match(std::get<0>(name), MemberNameRegex)) {
			error("Invalid member name.", std::get<1>(name), std::get<2>(name));
		}

		*memberNode = new structure::MemberNode();
		parentNode->add(*memberNode);
		(*memberNode)->setName(std::get<0>(name));
		(*memberNode)->type = type;

		{
			std::map<std::string, std::string> outModifiers;
			std::vector<std::tuple<std::string, unsigned long, unsigned long>> otherModifiers;
			groupModifiers(modifiers, {
					{"visibility", {"public", "protected", "private"}},
					{"staticality", {"~dynamic", "static"}}
				}, outModifiers,
				otherModifiers);

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

		end(*memberNode);
	}
} }
