#include "ImportParser.h"
#include "regex/regex.h"

namespace yaplc { namespace parser {
	void ImportParser::handle(structure::Listable *parentNode) {
		skipEmpty();

		begin();
		if (!parseImport(parentNode)) {
			cancel();
		}

		for (auto node : parsedNodes) {
			end(node);
		}
	}

	bool ImportParser::parseImport(structure::Listable *parentNode) {
		try {
			skipOrFail("import", "");
		} catch (...) {
			return false;
		}

		parseSubImport(parentNode, "", false);

		return true;
	}

	void ImportParser::parseSubImport(structure::Listable *parentNode, const std::string &prefix, bool parentStatic) {
		bool importStatic = parentStatic;
		std::string importName;

get_import:
		skipEmpty();

		if (!get("([A-Za-z0-9\\.\\$_]*)", {&importName})) {
			error("Expected import name.");
			cancelFatal();
		}

		if ((!importStatic) && (importName == "static")) {
			importStatic = true;

			goto get_import;
		}

		if (!regex::match("^([a-zA-Z\\$_][a-zA-Z0-9\\$_]*\\.)*[a-zA-Z\\$_][a-zA-Z0-9\\$_]*$", importName)) {
			error("Invalid import name.", position() - importName.size(), position() - 1);
		}

		importName = prefix + importName;

		skipEmpty();

		switch (get()) {
		case '{':
			skip();
			skipEmpty();

			while (get() != '}') {
				parseSubImport(parentNode, importName + ".", importStatic);
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
				std::vector<std::string> caps;
				regex::match("([A-Za-z0-9]*)$", importName, caps, 1);

				auto importNode = new structure::ImportNode();
				importNode->isStatic = importStatic;
				importNode->name = caps[0];
				importNode->target = importName;
				parentNode->add(importNode);
				parsedNodes.push_back(importNode);

				if (get() == ';') {
					skip();
				}
				break;
			} else if (skip("as")) {
				std::string name;
				
				if (!getWord(name)) {
					error("Expected name after 'as'.");
					cancelFatal();
				}

				auto importNode = new structure::ImportNode();
				importNode->isStatic = importStatic;
				importNode->name = name;
				importNode->target = importName;
				parentNode->add(importNode);
				parsedNodes.push_back(importNode);

				if (prefix == "") {
					expected(';');
				}
				
				break;
			}

			error(std::string("Expected 'as' or ';' or '{'. Got '") + get() + "'.");
			cancelFatal();
		}
	}
} }
