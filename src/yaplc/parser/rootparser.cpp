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
			if (!get("([A-Za-z0-9\\.]*)", {&packageName})) {
				error("Expected package name.");
				cancelFatal();
			}

			if (!regex::match("^([a-zA-Z][a-zA-Z0-9]*\\.)*[a-zA-Z][a-zA-Z0-9]*$", packageName)) {
				error("Invalid package name.", position() - packageName.size(), position() - 1);
			}
			
			skipEmpty();
			
			auto packageNode = new structure::PackageNode();
			rootNode->add(packageName, packageNode);
			
			switch (get()) {
			case ';': {
				skip();

				structure::Node *type;

				if (!parse<TypeParser>(&type)) {
					error("Type expected.");
					cancel();
				}

				packageNode->add(type);
				
				goto done;
			}
			case '{': {
				skip();
				while (true) {
					structure::Node *type;

					if (!parse<TypeParser>(&type)) {
						break;
					}

					packageNode->add(type);
				}

				if (get() != '}') {
					error(std::string("Expected '}'. Got '") + get() + "'.");
					cancelFatal();
				}

				skip();
				break;
			}
			default:
				error(std::string("Expected ';' or '{'. Got '") + get() + "'.");
				cancelFatal();
			}
			
			push();
		}
		
done:
		skipEmpty();
		
		if (!end()) {
			error("Unexpected content.", position(), length() - 1);
			cancelFatal();
		}
	}
} }
