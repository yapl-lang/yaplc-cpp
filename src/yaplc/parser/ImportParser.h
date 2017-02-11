#pragma once

#include "Parser.h"
#include "yaplc/structure/Childable.h"

namespace yaplc { namespace parser {
	class ImportParser : public Parser<structure::Childable *> {
	protected:
		virtual void handle(structure::Childable *parentNode);

		bool parseImport(structure::Childable *parentNode);
		void parseSubImport(structure::Childable *parentNode, const std::string &prefix, bool parentStatic);
	};
} }
