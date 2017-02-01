#pragma once

#include <vector>
#include <string>
#include "configuration.h"
#include "parsermanager.h"
#include "cancelparseexception.h"
#include "cancelfatalparseexception.h"

namespace yaplc { namespace parser {
	class BaseParser {
		friend class ParserManager;
	protected:
		ParserManager *parserManager;
		Configuration configuration, comingConfiguration;
		std::vector<Configuration> stack;
	
	protected:
		inline unsigned long position() const {
			return configuration.position;
		}
		
		inline unsigned long length() const {
			return configuration.code->size();
		}

	public:
		BaseParser();
		
	private:
		void setup(ParserManager *parserManager);
		
	protected:
		template<class T, class... Args> bool parse(Args... args) {
			auto parser = parserManager->getParser<T>();
			
			try {
				parser->handleParsing(configuration, args...);
			} catch (const CancelParseException &) {
				configuration = parser->comingConfiguration;
				delete parser;
				
				return false;
			} catch (const CancelFatalParseException &e) {
				throw e;
			}
			
			configuration = parser->configuration;
			
			delete parser;
			return true;
		}
		
		bool end();
		
		char get();
		std::string get(unsigned long count);
		bool get(const std::string &pattern, unsigned long count = 0, ...);
		bool getWord(const std::string &word);
		bool getLowercaseWord(const std::string &word);
		bool getModifiers(const std::map<std::string, std::vector<std::string>> &allowedModifiers, std::map<std::string, std::string> &modifiers);
		bool getName(std::string &name, const std::string &prevWord);
		bool getName(std::string &name, const std::string &prevWord, const std::map<std::string, std::vector<std::string>> &allowedModifiers, std::map<std::string, std::string> &modifiers);
		
		void skip(unsigned long count = 1);
		void skipSpaces();
		void skipEmpty();
		void skipOrFail(const std::string &string, const std::string &error = "");
		
		void push();
		void save();
		void restore();
		void norestore();
		bool cancel();
		bool cancelIfEnd();
		bool cancelFatal();
		
		bool error(const std::string &message);
		bool error(const std::string &message, unsigned long position);
		bool error(const std::string &message, unsigned long begin, unsigned long end);
	};

	template<class... Args> class Parser : public BaseParser {
		friend class ParserManager;
		friend class BaseParser;
	private:
		inline void handleParsing(const Configuration &configuration, Args... args) {
			this->configuration = comingConfiguration = configuration;
			handle(args...);
		}
		
	protected:
		virtual void handle(Args... args) = 0;
	};
} }
