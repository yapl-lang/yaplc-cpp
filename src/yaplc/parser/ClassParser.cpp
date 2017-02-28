#include "ClassParser.h"
#include "MemberParser.h"
#include "yaplc/structure/ClassNode.h"
#include "yaplc/structure/MemberNode.h"
#include "TypeNameParser.h"
#include "SpecialParser.h"
#include <algorithm>

namespace yaplc { namespace parser {
	void ClassParser::handle(structure::ClassNode *classNode) {
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

				structure::TypeNameNode *interface = nullptr;
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

		while ((parse<SpecialParser>(classNode)) || (parse<MemberParser>(classNode)));
		
		skipEmpty();
		
		if (get() != '}') {
			error(std::string("Expected '}'. Got '") + get() + "'.");
			cancelFatal();
		}
		
		skip();
	}
} }
