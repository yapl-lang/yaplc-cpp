#pragma once

#include "Parser.h"
#include "yaplc/structure/RootNode.h"
#include "yaplc/structure/PackageNode.h"

namespace yaplc { namespace parser {
	class RootParser : public Parser<structure::RootNode *> {
	protected:
		virtual void handle(structure::RootNode *rootNode);

		void parsePackageBody(structure::PackageNode *packageNode);
		bool parseImport(structure::PackageNode *packageNode);
		void parseSubImport(structure::PackageNode *packageNode, const std::string &prefix, bool parentStatic);
	};
} }
