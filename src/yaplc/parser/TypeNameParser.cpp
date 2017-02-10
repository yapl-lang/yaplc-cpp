#include "TypeNameParser.h"

namespace yaplc { namespace parser {
	void TypeNameParser::handle(structure::TypeNameNode **typeNameNode) {
		std::string type;

		if (!getWord(type)) {
			cancel();
		}

		auto node = new structure::TypeNameNode();
		*typeNameNode = node;

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
					structure::TypeNameNode *templateArgument;

					if (!parse<TypeNameParser>(&templateArgument)) {
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
