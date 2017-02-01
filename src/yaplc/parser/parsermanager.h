#pragma once

#include <string>
#include <vector>
#include <map>
#include "yaplc/structure/rootnode.h"

namespace yaplc {
	class CompilingError;
}

namespace yaplc { namespace parser {
	class SyntaxError;
	
	class ParserManager {
		friend class BaseParser;
	private:
		std::vector<CompilingError *> *errors;
		
	public:
		ParserManager();
		
		template<class T> inline T *getParser() {
			auto parser = new T();
			parser->setup(this);
			return parser;
		}
		
		structure::RootNode *parse(const std::string &code, std::vector<CompilingError *> &errors);
		
	protected:
		virtual void handle() {}
	
	private:
		void pushError(SyntaxError *error);
	};
} }
