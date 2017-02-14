#include "TypeParser.h"
#include "ClassParser.h"

namespace yaplc { namespace parser {
	void TypeParser::handle(structure::Listable *parentNode) {
		cancelIfEnd();
		
		static std::vector<std::string> typeNames = {
			"class",
			"interface",
			"struct",
		};
		
		std::string typeName;
		std::map<std::string, std::string> modifiers;
		std::vector<std::tuple<std::string, unsigned long, unsigned long>> otherModifiers;
		getModifiers(typeNames, typeName, {
			{"visibility", {"public", "protected", "private"}}
		}, modifiers, otherModifiers);
		
		if (typeName == "class") {
			parse<ClassParser>(parentNode) || (error("Class expected.") && cancel());
		} else if (typeName == "interface") {
			cancel(); // TODO: Implement
		} else if (typeName == "struct") {
			cancel(); // TODO: Implement
		} else {
			cancel();
		}
	}
} }
