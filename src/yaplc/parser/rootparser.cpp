#include "rootparser.h"
#include "regex/regex.h"
#include "typeparser.h"
#include "yaplc/structure/packagenode.h"
#include "yaplc/structure/importnode.h"

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

			unsigned long packageNameEnding = position();
			skipEmpty();

			auto packageNode = new structure::PackageNode();
			rootNode->add(packageName, packageNode);

			switch (get()) {
			case ';': {
				skip();

				parsePackageBody(packageNode);

				goto done;
			}
			case '{': {
				skip();

				parsePackageBody(packageNode);

				if (get() != '}') {
					error(std::string("Expected '}'. Got '") + get() + "'.");
					cancelFatal();
				}

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
		while ((parse<TypeParser>(packageNode)) || (parseImport(packageNode)));
	}

	bool RootParser::parseImport(structure::PackageNode *packageNode) {
		try {
			skipOrFail("import", "");
		} catch (...) {
			return false;
		}

		parseSubimport(packageNode, "", false);

		return true;
	}

	void RootParser::parseSubimport(structure::PackageNode *packageNode, const std::string &prefix, bool parentStatic) {
		bool importStatic = parentStatic;
		std::string importName;

get_import:
		skipEmpty();

		if (!get("([A-Za-z0-9\\.]*)", {&importName})) {
			error("Expected import name.");
			cancelFatal();
		}

		if ((!importStatic) && (importName == "static")) {
			importStatic = true;

			goto get_import;
		}

		if (!regex::match("^([a-zA-Z][a-zA-Z0-9]*\\.)*[a-zA-Z][a-zA-Z0-9]*$", importName)) {
			error("Invalid import name.", position() - importName.size(), position() - 1);
		}

		importName = prefix + importName;

		skipEmpty();

		switch (get()) {
		case '{':
			skip();
			skipEmpty();

			while (get() != '}') {
				parseSubimport(packageNode, importName + ".", importStatic);
				skipEmpty();

				switch (get()) {
				case ',':
					skip();
				case '}':
					break;
				default:
					error(std::string("Expected ',' or '}'. Got '") + get() + "'.");
					cancelFatal();
				}
			}

			expected('}');
			break;
		default:
			if ((get() == ';') || (prefix != "")) {
				auto importNode = new structure::ImportNode();
				importNode->isStatic = importStatic;
				packageNode->add(importName, importNode);

				if (get() == ';') {
					skip();
				}
				break;
			}

			error(std::string("Expected ';' or '{'. Got '") + get() + "'.");
			cancelFatal();
		}
	}
} }
