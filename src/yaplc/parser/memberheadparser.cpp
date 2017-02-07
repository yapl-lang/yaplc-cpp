#include "memberheadparser.h"
#include "regex/regex.h"
#include "yaplc/util/map.h"

namespace yaplc { namespace parser {
	void MemberHeadParser::handle(structure::MemberNode *memberNode) {
		std::vector<std::pair<std::string, std::pair<unsigned long, unsigned long>>> modifiers;

		if (get("[A-Za-z0-9_]*", modifiers) < 2) {
			cancel();
		}

		std::pair<std::string, std::pair<unsigned long, unsigned long>>
			name, type;
		
		auto it = modifiers.end();
		name = (*(--it));
		type = (*(--it));
		modifiers.resize(modifiers.size() - 2);

		for (auto modifier : modifiers) {
			if (!regex::match("[A-Za-z_][A-Za-z0-9_]*", modifier.first)) {
				error("Invalid modifier name.", modifier.second.first, modifier.second.second);
			}
		}

		if (!regex::match("[A-Za-z_][A-Za-z0-9_]*", type.first)) {
			error("Invalid type name.", type.second.first, type.second.second);
		}

		if (!regex::match("[A-Za-z_][A-Za-z0-9_]*", name.first)) {
			error("Invalid member name.", name.second.first, name.second.second);
		}
		
		memberNode->setName(name.first);
		memberNode->type = type.first;
		util::map
			<std::pair<std::string, std::pair<unsigned long, unsigned long>>, std::string>
			(modifiers, memberNode->modifiers, [](std::pair<std::string, std::pair<unsigned long, unsigned long>> value, unsigned long) {
			return value.first;
		});
	}
} }
