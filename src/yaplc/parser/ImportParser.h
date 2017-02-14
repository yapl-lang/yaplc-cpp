#pragma once

#include "Parser.h"
#include "yaplc/structure/Childable.h"

namespace yaplc { namespace parser {
	class ImportParser : public Parser<structure::Listable *> {
	protected:
		virtual void handle(structure::Listable *parentNode);

		bool parseImport(structure::Listable *parentNode);
		void parseSubImport(structure::Listable *parentNode, const std::string &prefix, bool parentStatic);
	};
} }
