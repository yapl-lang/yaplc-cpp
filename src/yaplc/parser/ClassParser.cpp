#include "ClassParser.h"
#include "MemberParser.h"
#include "yaplc/structure/ClassNode.h"
#include "yaplc/structure/MemberNode.h"
#include "TypeNameParser.h"
#include <algorithm>

namespace yaplc { namespace parser {
	void ClassParser::handle(structure::Childable *parentNode) {
		skipEmpty();
		push();
		
		std::map<std::string, std::string> classModifiers;
		
		std::vector<std::string> possibleVisibility;
		(true) ?
			(possibleVisibility = {"public", "private"}) :
			(possibleVisibility = {"public", "protected", "private"});
		
		std::string className;
		if (!getName(className, "class", {
			{"visibility", possibleVisibility}
		}, classModifiers)) {
			cancel();
		}
		
		auto classNode = new structure::ClassNode(className);
		parentNode->add(classNode);
		
		{
			std::string visibility = classModifiers["visibility"];
			
			if (visibility == "public") {
				classNode->visibility = structure::ClassNode::Visibility::Public;
			} else if (visibility == "protected") {
				classNode->visibility = structure::ClassNode::Visibility::Protected;
			} else if (visibility == "private") {
				classNode->visibility = structure::ClassNode::Visibility::Private;
			}
		}
		
		skipEmpty();
		
		if (skip("extends")) {
			if (!parse<TypeNameParser>(&classNode->base)) {
				error("Type name expected.");
				cancelFatal();
			}
		}

		if (skip("implements")) {
			bool first = true;

			while (true) {
				skipEmpty();
				if (first) {
					first = false;
				} else {
					if (get() != ',') {
						norestore();

						break;
					}

					skip();
				}

				skipEmpty();

				structure::TypeNameNode *interface;
				if (!parse<TypeNameParser>(&interface)) {
					break;
				}

				classNode->interfaces.push_back(interface);
			}
		}
		
		skipEmpty();
		
		if (get() != '{') {
			error(std::string("Expected '{'. Got '") + get() + "'.");
			cancelFatal();
		}
		
		skip();
		skipEmpty();


		while (parse<MemberParser>(classNode));
		
		skipEmpty();
		
		if (get() != '}') {
			error(std::string("Expected '}'. Got '") + get() + "'.");
			cancelFatal();
		}
		
		skip();
	}
} }
