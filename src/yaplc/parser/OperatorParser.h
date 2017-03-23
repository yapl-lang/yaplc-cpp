#pragma once

#include "Parser.h"
#include "yaplc/structure/OperatorNode.h"
#include "yaplc/structure/Listable.h"
#include <regex>

namespace yaplc { namespace parser {
	class OperatorParser : public Parser<structure::Listable *> {
	protected:
		static std::regex WhitespaceRegex;

		virtual void handle(structure::Listable *parentNode);
		void parseArguments(structure::OperatorNode *node);
	};
} }
