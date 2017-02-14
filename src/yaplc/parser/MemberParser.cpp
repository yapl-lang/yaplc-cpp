#include "MemberParser.h"
#include "TypeParser.h"
#include "MemberHeadParser.h"
#include "VariableMemberParser.h"
#include "MethodMemberParser.h"
#include "yaplc/structure/ClassNode.h"

namespace yaplc { namespace parser {
	void MemberParser::handle(structure::Listable *parentNode) {
		skipEmpty();
		push();

		bool isClass = dynamic_cast<structure::ClassNode *>(parentNode) != nullptr;
		bool isInterface = false;
		//bool isInterface = dynamic_cast<structure::InterfaceNode *>(parentNode) != nullptr;
		bool isStruct = false;
		//bool isStruct = dynamic_cast<structure::StructNode *>(parentNode) != nullptr;
		
		if (end()) {
			cancel();
		}

		if (parse<TypeParser>(parentNode)) {
			return;
		}

		structure::MemberNode *memberNode;
		if (!parse<MemberHeadParser>(parentNode, &memberNode)) {
			cancel();
		}

		parse<MethodMemberParser>(memberNode, isInterface || isStruct)
		|| ((isClass || isStruct) && parse<VariableMemberParser>(memberNode))
		|| cancel();
	}
} }
