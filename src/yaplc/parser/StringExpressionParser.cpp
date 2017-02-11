#include "StringExpressionParser.h"
#include "ExpressionParser.h"
#include "yaplc/structure/StringNode.h"
#include "yaplc/structure/CharNode.h"

namespace yaplc { namespace parser {
	void StringExpressionParser::handle(structure::ExpressionNode *parentNode) {
		cancelIfEnd();
		
		switch (get()) {
		case '"':
			parseString(parentNode);
			break;
		case '\'':
			parseChar(parentNode);
			break;
		case '`': {
			auto group = new structure::ExpressionNode();
			parentNode->add(group);
			parseTemplateString(group);
			break;
		}
		default:
			cancel();
		}
	}
	
	void StringExpressionParser::parseString(structure::ExpressionNode *parentNode) {
		skip();
		
		std::string result;
		bool escape = false;
		
		while (true) {
			switch (char c = getAndSkip()) {
			case '"':
				if (escape) {
					result += "\"";
					escape = false;
				} else {
					goto done;
				}
				break;
			case '\\':
				if (!(escape = !escape)) {
					result += "\\";
				}
				break;
			default:
				escape = false;
				result += c;
			}
		}
		
done:
		auto node = new structure::StringNode();
		node->value = result;
		parentNode->add(node);
	}
	
	void StringExpressionParser::parseChar(structure::ExpressionNode *parentNode) {
		skip();
		
		std::string result;
		bool escape = false;
		
		while (true) {
			switch (char c = getAndSkip()) {
			case '\'':
				if (escape) {
					result += "'";
					escape = false;
				} else {
					goto done;
				}
				break;
			case '\\':
				if (!(escape = !escape)) {
					result += "\\";
				}
				break;
			default:
				escape = false;
				result += c;
			}
		}
		
done:
		if (result.size() != 1) {
			error("Char literal must contain only one character.", position() - result.length() - 2, position() - 1);
		}
		
		auto node = new structure::CharNode();
		node->value = result;
		parentNode->add(node);
	}
	
	void StringExpressionParser::parseTemplateString(structure::ExpressionNode *parentNode) {
		skip();
		
		std::string result;
		bool escape = false;
		
		while (true) {
			switch (char c = getAndSkip()) {
			case '`':
				if (escape) {
					result += "`";
					escape = false;
				} else {
					goto done;
				}
				break;
			case '\\':
				if (!(escape = !escape)) {
					result += "\\";
				}
				break;
			case '$':
				if (!escape) {
					if (get() != '{') {
						back();
						
						result += "$";
						break;
					}
					
					{
						auto node = new structure::StringNode();
						node->value = result;
						parentNode->add(node);
						
						result = "";
					}
					
					skip();
					
					auto group = new structure::ExpressionNode();
					parentNode->add(group);
					
					parse<ExpressionParser>(group, true);
					
					expected('}');
				}
				break;
			default:
				escape = false;
				result += c;
			}
		}
		
done:
		if (result != "") {
			auto node = new structure::StringNode();
			node->value = result;
			parentNode->add(node);
		}
	}
} }
