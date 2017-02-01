#pragma once

#include "parser.h"

namespace yaplc { namespace parser {
	class CommentsParser : public Parser<> {
		virtual void handle();
	
	private:
		void parseLineComment();
	};
} }
