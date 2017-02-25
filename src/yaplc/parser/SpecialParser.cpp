#include "SpecialParser.h"
#include "StringExpressionParser.h"
#include "yaplc/structure/SpecialNode.h"
#include "yaplc/structure/StringNode.h"
#include "yaplc/structure/Listable.h"
#include "yaplc/structure/Childable.h"
#include "yaplc/structure/Container.h"

namespace yaplc { namespace parser {
	void SpecialParser::handle(structure::Node *parentNode) {
		if (!skip("#special")) {
			cancel();
		}

		auto exp = new structure::ExpressionNode();
		begin();
		if (!parse<StringExpressionParser>(exp)) {
			error("Special data expected.");
			delete exp;
			cancelFatal();
		}
		end(exp);

		if (exp->end() - exp->begin() != 1) {
			error("Special data expected.", exp->getBegin(), exp->getEnd());
			delete exp;
			cancelFatal();
		}

		auto node = new structure::SpecialNode();
		node->data = ((structure::StringNode *)(*exp->begin()))->value;
		delete exp;

		if (auto listable = dynamic_cast<structure::Listable *>(parentNode)) {
			listable->add(node);
		} else if (auto childable = dynamic_cast<structure::Childable *>(parentNode)) {
			childable->add(node);
		} else if (auto container = dynamic_cast<structure::Container *>(parentNode)) {
			container->set(node);
		} else {
			delete node;
		}
	}
} }
