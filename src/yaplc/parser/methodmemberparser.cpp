#include "methodmemberparser.h"
#include "yaplc/structure/variablemembernode.h"

namespace yaplc { namespace parser {
	void MethodMemberParser::handle(structure::Node **node) {
		/*std::map<std::string, std::string> modifiers;
		
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
		
		auto methodMemberNode = new structure::MemberNode(name);
		*node = methodMemberNode
		
		methodMemberNode->type = type;
		
		{
			std::string visibility = modifiers["visibility"];
			
			if (visibility == "public") {
				methodMemberNode->visibility = structure::VariableMemberNode::Visibility::Public;
			} else if (visibility == "protected") {
				methodMemberNode->visibility = structure::VariableMemberNode::Visibility::Protected;
			} else if (visibility == "private") {
				methodMemberNode->visibility = structure::VariableMemberNode::Visibility::Private;
			}
		}
		
		{
			std::string staticality = modifiers["staticality"];
			
			if (staticality == "dynamic") {
				methodMemberNode->staticality = structure::VariableMemberNode::Staticality::Dynamic;
			} else if (staticality == "static") {
				methodMemberNode->staticality = structure::VariableMemberNode::Staticality::Static;
			}
		}
		
		
		skipEmpty();*/
		cancel();
	}
} }
