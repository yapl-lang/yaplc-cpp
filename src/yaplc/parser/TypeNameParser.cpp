#include "TypeNameParser.h"

namespace yaplc { namespace parser {
	void TypeNameParser::handle(structure::TypeNameNode **typeNameNode) {
		std::string type;

		if (!getWord(type)) {
			cancel();
		}

		auto node = *typeNameNode;
		if (node == nullptr) {
			auto node = new structure::TypeNameNode();
			*typeNameNode = node;
		}
		
		node->type = type;

		save();
		skipEmpty();

		// Check if we will parse a template
		if (get() == '<') {
			norestore();

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
				delete node;
				*typeNameNode = nullptr;
				cancelFatal();
			}

			skip();
		} else {
			restore();
		}
	}
} }
