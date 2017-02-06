#include "memberheadparser.h"
#include "regex/regex.h"

namespace yaplc { namespace parser {
	void MemberHeadParser::handle(structure::MemberNode *memberNode) {
		std::map<std::string, std::pair<unsigned long, unsigned long>> modifiers;

		if (get("[A-Za-z0-9_]", modifiers) < 2) {
			cancel();
		}

		auto name = modifiers.pop_back();
		auto type = modifiers.pop_back();

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
	}
} }
