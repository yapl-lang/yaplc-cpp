#include "typereferenceparser.h"

namespace yaplc { namespace parser {
	void TypeReferenceParser::handle(structure::TypeReferenceNode **typeReferenceNode) {
		std::string type;

		if (!getWord(type)) {
			cancel();
		}

		auto node = new structure::TypeReferenceNode();
		*typeReferenceNode = node;

		node->type = type;

		skipEmpty();

		// Check if we will parse a template
		if (get() == '<') {
			skip();
			skipEmpty();

			while (true) {
				switch (get()) {
				case '>':
					goto end;
				default:
					structure::TypeReferenceNode *templateArgument;

					if (!parse<TypeReferenceParser>(&templateArgument)) {
						goto end;
					}

					node->templateArguments.push_back(templateArgument);

					skipEmpty();

					if (get() == ',') {
						skip();
						skipEmpty();
					}
				}
			}

end:
			skipEmpty();
			if (get() != '>') {
				error(std::string("Expected '>'. Got '") + get() + "'.");
				cancelFatal();
			}

			skip();
		}
	}
} }
