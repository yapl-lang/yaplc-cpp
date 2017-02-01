#include "numberexpressionparser.h"

namespace yaplc { namespace parser {
	void NumberExpressionParser::handle(structure::Node *&node) {
		cancelIfEnd();
		
		std::string number;
		
		if (!get("([0-9]*(\\.[0-9]*)?)", 2, &number, nullptr)) {
			cancel();
		}
		
		switch (get()) {
		case ';':
			break;
		}
	}
} }
