#include "RootParser.h"
#include "TypeParser.h"
#include "ImportParser.h"

namespace yaplc { namespace parser {
	std::regex RootParser::SimplePackageNameRegex{"([A-Za-z0-9\\.\\$_]*)"};
	std::regex RootParser::PackageNameCheckRegex{"^([a-zA-Z\\$_][a-zA-Z\\$_0-9]*\\.)*[a-zA-Z\\$_][a-zA-Z0-9\\$_]*$"};

	void RootParser::handle(structure::RootNode *rootNode) {
		while (true) {
			skipEmpty();
			
			if (end()) {
				push();
				
				break;
			}

			begin();
			skipOrFail("package", "Expected 'package'.");

			skipEmpty();

			std::smatch match;

			if (!get(SimplePackageNameRegex, match)) {
				error("Expected package name.");
				cancelFatal();
			}

			auto packageName = match.str();

			if (!std::regex_match(packageName, PackageNameCheckRegex)) {
				error("Invalid package name.", position() - packageName.size(), position() - 1);
			}

			unsigned long packageNameEnding = position();
			skipEmpty();

			auto packageNode = new structure::PackageNode();
			packageNode->name = packageName;
			rootNode->add(packageNode);

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
