#include "memberparser.h"
#include "typeparser.h"
#include "memberheadparser.h"
#include "variablememberparser.h"
#include "methodmemberparser.h"
#include "yaplc/structure/classnode.h"

namespace yaplc { namespace parser {
	void MemberParser::handle(structure::Childable *parentNode) {
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
		//|| parse<VariableMemberParser>(node)
		|| cancel();
	}
} }
