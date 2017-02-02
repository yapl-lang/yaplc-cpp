#include "typeparser.h"
#include "classparser.h"

namespace yaplc { namespace parser {
	void TypeParser::handle(structure::Node **node) {
		cancelIfEnd();
		
		parse<ClassParser>(node)
		|| cancel();
	}
} }
