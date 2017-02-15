#include "RootParser.h"
#include "regex/regex.h"
#include "TypeParser.h"
#include "ImportParser.h"

namespace yaplc { namespace parser {
	void RootParser::handle(structure::RootNode *rootNode) {
		while (true) {
			skipEmpty();
			
			if (end()) {
				push();
				
				break;
			}

			auto packageStart = position();
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

			unsigned long packageNameEnding = position();
			skipEmpty();

			auto packageNode = new structure::PackageNode();
			rootNode->add(packageNode);
			packageNode->setBegin(packageStart);

			switch (get()) {
			case ';': {
				skip();

				parsePackageBody(packageNode);
				end(packageNode);

				goto done;
			}
			case '{': {
				skip();

				parsePackageBody(packageNode);

				if (get() != '}') {
					error(std::string("Expected '}'. Got '") + get() + "'.");
					cancelFatal();
				}
				
				end(packageNode);

				skip();
				break;
			}
			default:
				error(std::string("Expected ';' or '{'. Got '") + get() + "'.", packageNameEnding, position());
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

	void RootParser::parsePackageBody(structure::PackageNode *packageNode) {
		while (true) {
			if (parse<ImportParser>(packageNode)) {
				continue;
			}
			
			if (parse<TypeParser>(packageNode)) {
				continue;
			}
			
			break;
		}
	}
} }
