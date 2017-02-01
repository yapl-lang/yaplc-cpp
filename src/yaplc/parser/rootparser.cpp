#include "rootparser.h"
#include "regex/regex.h"
#include "typeparser.h"
#include "yaplc/structure/packagenode.h"

namespace yaplc { namespace parser {
	void RootParser::handle(structure::RootNode *rootNode) {
		while (true) {
			skipEmpty();
			
			if (end()) {
				push();
				
				break;
			}
			
			skipOrFail("package", "Expected 'package'.");
			
			skipEmpty();
			
			std::string packageName;
			get("([A-Za-z0-9\\.]*)", 1, &packageName);
			
			if (!regex::match("^([a-zA-Z][a-zA-Z0-9]*\\.)*[a-zA-Z][a-zA-Z0-9]*$", packageName)) {
				error("Invalid package name.", position() - packageName.size(), position() - 1);
			}
			
			skipEmpty();
			
			auto packageNode = new structure::PackageNode();
			rootNode->add(packageName, packageNode);
			
			switch (get()) {
			case ';': {
				skip();
				if (!parse<TypeParser>(packageNode)) {
					// TODO:
				}
				
				goto done;
			}
			case '{': {
				skip();
				while (parse<TypeParser>(packageNode));
				// TODO: }
				break;
			}
			default:
				error(std::string("Expected ';' or '{'. Got '") + get() + "'.");
			}
			
			push();
		}
		
done:
		skipEmpty();
		
		if (!end()) {
			error("Unexpected content.", position(), length() - 1);
		}
	}
} }
