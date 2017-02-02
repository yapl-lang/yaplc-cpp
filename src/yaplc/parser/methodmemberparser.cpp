#include "methodmemberparser.h"
#include "yaplc/structure/variablemembernode.h"

namespace yaplc { namespace parser {
	void MethodMemberParser::handle(structure::Childable *parentNode) {
		std::map<std::string, std::string> modifiers;
		
		if (!getModifiers({
			{"visibility", {"private", "public", "protected"}},
			{"staticality", {"~dynamic", "static"}},
			{"virtuality", {"virtual", "~static"}}
		}, modifiers)) {
			cancel();
		}
		
		std::string type, name;
		error("OK");
		if ((!getWord(type)) || (!getWord(name))) {
			cancel();
		}
		
		auto variableMemberNode = new structure::VariableMemberNode();
		parentNode->add(name, variableMemberNode);
		
		variableMemberNode->type = type;
		
		{
			std::string visibility = modifiers["visibility"];
			
			if (visibility == "public") {
				variableMemberNode->visibility = structure::VariableMemberNode::Visibility::Public;
			} else if (visibility == "protected") {
				variableMemberNode->visibility = structure::VariableMemberNode::Visibility::Protected;
			} else if (visibility == "private") {
				variableMemberNode->visibility = structure::VariableMemberNode::Visibility::Private;
			}
		}
		
		{
			std::string staticality = modifiers["staticality"];
			
			if (staticality == "dynamic") {
				variableMemberNode->staticality = structure::VariableMemberNode::Staticality::Dynamic;
			} else if (staticality == "static") {
				variableMemberNode->staticality = structure::VariableMemberNode::Staticality::Static;
			}
		}
		
		
		skipEmpty();
	}
} }
