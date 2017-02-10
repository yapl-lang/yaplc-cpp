#include "TypeParser.h"
#include "ClassParser.h"

namespace yaplc { namespace parser {
	void TypeParser::handle(structure::Childable *parentNode) {
		cancelIfEnd();
		
		parse<ClassParser>(parentNode)
		|| cancel();
	}
} }
