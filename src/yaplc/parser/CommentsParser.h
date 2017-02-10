#pragma once

#include "Parser.h"

namespace yaplc { namespace parser {
	class CommentsParser : public Parser<> {
		virtual void handle();
	
	private:
		void parseLineComment();
	};
} }
