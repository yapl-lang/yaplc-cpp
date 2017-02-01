#include "commentsparser.h"

namespace yaplc { namespace parser {
	void CommentsParser::handle() {
		while (true) {
			save();
			skipSpaces();
			
			if (end()) {
				restore();
				
				return;
			}
			
			switch (get()) {
			case '#':
				skip();
				parseLineComment();
				
				push();
				break;
			case '/':
				skip();
				
				switch (get()) {
				case '/':
					skip();
					parseLineComment();
					push();
					break;
				case '*':
					skip();
					
					try {
						while (get(2) != "*/") {
							skip();
						}
						
						skip(2);
						push();
					} catch (...) {
						error("Unexpected end of file. Missing '*/'.");
						cancelFatal();
					}
					break;
				default:
					restore();
					return;
				}
				break;
			default:
				restore();
				return;
			}
		}
	}
	
	void CommentsParser::parseLineComment() {
		while (true) {
			if (end()) {
				return;
			}
			
			char ch = get();
			
			if ((ch == '\n') || (end())) {
				return;
			}
			
			skip();
			
			if ((ch == '\\') && (get() == '\n')) {
				skip();
			}
		}
	}
} }
