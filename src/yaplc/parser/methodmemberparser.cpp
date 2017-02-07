#include "methodmemberparser.h"
#include "yaplc/structure/methodmembernode.h"

namespace yaplc { namespace parser {
	void MethodMemberParser::handle(structure::MemberNode *parentNode) {
		skipEmpty();
		push();

		if (get() != '(') {
			cancel();
		}
		skip();

		auto methodMemberNode = new structure::MethodMemberNode();
		parentNode->set(methodMemberNode);



		/*
		
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
	}
} }
