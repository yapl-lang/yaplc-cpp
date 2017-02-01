#include "parsermanager.h"
#include "configuration.h"

#include "rootparser.h"
#include "commentsparser.h"

#include "yaplc/structure/rootnode.h"

#include "cancelfatalparseexception.h"
#include "cancelparseexception.h"
#include "../fatalerror.h"

#include <stdexcept>

namespace yaplc { namespace parser {
	ParserManager::ParserManager() {
		
	}
	
	structure::RootNode *ParserManager::parse(const std::string &code, std::vector<CompilingError *> &errors) {
		this->errors = &errors;
		
		auto configuration = Configuration();
		configuration.code = &code;
		
		RootParser *parser;
		auto rootNode = new structure::RootNode();
		
		try {
			parser = getParser<RootParser>();
			parser->handleParsing(configuration, rootNode);
		} catch (const CancelFatalParseException &e) {
			errors.push_back(new FatalError("Compilation cancelled."));
		} catch (const CancelParseException &e) {
			
		} catch (const std::exception &e) {
			errors.push_back(new FatalError(std::string("STD Exception: '") + e.what() + "'."));
		}
		
		delete parser;
		
		return rootNode;
	}
	
	void ParserManager::pushError(SyntaxError *error) {
		errors->push_back((CompilingError *)error);
	}
} }
