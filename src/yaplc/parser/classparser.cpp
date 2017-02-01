#include "classparser.h"
#include "memberparser.h"
#include "yaplc/structure/classnode.h"
#include "regex/regex.h"
#include <algorithm>

namespace yaplc { namespace parser {
	void ClassParser::handle(structure::Childable *packageNode) {
		skipEmpty();
		push();
		
		std::map<std::string, std::string> classModifiers;
		
		std::vector<std::string> possibleVisibility;
		(dynamic_cast<structure::ClassNode *>(packageNode)) ?
			(possibleVisibility = {"public", "protected", "private"}) :
			(possibleVisibility = {"public", "private"});
		
		std::string className;
		if (!getName(className, "class", {
			{"visibility", possibleVisibility}
		}, classModifiers)) {
			cancel();
		}
		
		auto classNode = new structure::ClassNode();
		packageNode->add(className, classNode);
		
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
		
		{
			std::string base;
			
			if (getName(base, "extends")) {
				classNode->base = base;
			}
		}
		
		{
			std::string interface;
			
			if (getName(interface, "implements")) {
				classNode->interfaces.push_back(interface);
				
				while (true) {
					save();
					
					skipEmpty();
					if (get() != ',') {
						norestore();
						
						break;
					}
					
					skip();
					skipEmpty();
					
					if (!getWord(interface)) {
						restore();
						
						break;
					}
					
					if (std::find(classNode->interfaces.begin(), classNode->interfaces.end(), interface) != classNode->interfaces.end()) {
						error("Cannot implement a interface more than one time.", position() - interface.length(), position() - 1);
					} else {
						classNode->interfaces.push_back(interface);
					}
					
					norestore();
				}
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
