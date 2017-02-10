#pragma once

#include "parser.h"
#include "yaplc/structure/rootnode.h"
#include "yaplc/structure/packagenode.h"

namespace yaplc { namespace parser {
	class RootParser : public Parser<structure::RootNode *> {
	protected:
		virtual void handle(structure::RootNode *rootNode);

		void parsePackageBody(structure::PackageNode *packageNode);
		bool parseImport(structure::PackageNode *packageNode);
		void parseSubimport(structure::PackageNode *packageNode, const std::string &prefix, bool parentStatic);
	};
} }
