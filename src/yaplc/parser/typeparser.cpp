#include "typeparser.h"
#include "classparser.h"

namespace yaplc { namespace parser {
	void TypeParser::handle(structure::Childable *parentNode) {
		cancelIfEnd();
		
		parse<ClassParser>(parentNode)
		|| cancel();
	}
} }
