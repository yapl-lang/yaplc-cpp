#pragma once

#include "Parser.h"
#include "yaplc/structure/ImportNode.h"
#include "yaplc/structure/Listable.h"
#include <regex>

namespace yaplc { namespace parser {
	class ImportParser : public Parser<structure::Listable *> {
	protected:
		static std::regex SimpleImportNameRegex;
		static std::regex ImportNameRegex;
		static std::regex LastNameRegex;

		std::vector<structure::ImportNode *> parsedNodes;

		virtual void handle(structure::Listable *parentNode);

		bool parseImport(structure::Listable *parentNode);
		void parseSubImport(structure::Listable *parentNode, const std::string &prefix, bool parentStatic);
	};
} }
