#include "TypeParser.h"
#include "ClassParser.h"
#include "TypeNameParser.h"

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

		skipEmpty();
		begin();
		if (!getModifiers(typeNames, typeName, {
			{"visibility", {"public", "protected", "private"}}
		}, modifiers, otherModifiers)) {
			cancel();
		}

		skip(typeName);

		structure::TypeNode *typeNode;

		if (typeName == "class") {
			auto classNode = new structure::ClassNode();
			typeNode = classNode;
			parentNode->add(typeNode);

			if (!parse<TypeNameParser>(&typeNode->name)) {
				error("Type name expected.") && cancelFatal();
			}

			parse<ClassParser>(classNode) || (error("Class expected.") && cancelFatal());
		} else if (typeName == "interface") {
			cancel(); // TODO: Implement
		} else if (typeName == "struct") {
			cancel(); // TODO: Implement
		} else {
			cancel();
		}

		end(typeNode);

		{
			auto visibility = modifiers["visibility"];

			if (visibility == "public") {
				typeNode->visibility = structure::TypeNode::Visibility::Public;
			} else if (visibility == "protected") {
				typeNode->visibility = structure::TypeNode::Visibility::Protected;
			} else if (visibility == "private") {
				typeNode->visibility = structure::TypeNode::Visibility::Private;
			}
		}
	}
} }
