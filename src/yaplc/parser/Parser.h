#pragma once

#include "Configuration.h"
#include "ParserManager.h"
#include "CancelParseException.h"
#include "CancelFatalParseException.h"
#include "yaplc/structure/Node.h"
#include <vector>
#include <string>
#include <map>
#include <tuple>

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
				delete parser;

				throw e;
			}
			
			configuration = parser->configuration;
			
			delete parser;
			return true;
		}
		
		inline void begin(structure::Node *node) {
			node->setBegin(position());
		}
		
		inline void end(structure::Node *node) {
			node->setEnd(position() - 1);
		}
		
		bool end();
		
		char get();
		char getAndSkip();
		std::string get(unsigned long count);
		bool get(const std::string &pattern);
		bool get(const std::string &pattern, const std::vector<std::string *> &caps);
		unsigned long get(const std::string &pattern, std::vector<std::string> &results);
		unsigned long get(const std::string &pattern, std::vector<std::tuple<std::string, unsigned long, unsigned long>> &results);
		bool getWord(std::string &word);
		bool getLowercaseWord(std::string &word);
		unsigned long getWords(std::vector<std::string> &words);
		
		bool getModifiers(const std::string &beforeWord,
		                  const std::map<std::string, std::vector<std::string>> &allowedModifiers,
		                  std::map<std::string, std::string> &outModifiers,
		                  std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers);
		bool getModifiers(const std::vector<std::string> &beforeWords,
		                  std::string &beforeWord,
		                  const std::map<std::string, std::vector<std::string>> &allowedModifiers,
		                  std::map<std::string, std::string> &outModifiers,
		                  std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers);
		bool getModifiers(const std::map<std::string, std::vector<std::string>> &allowedModifiers, std::map<std::string, std::string> &modifiers);
		bool getName(std::string &name, const std::string &prevWord);
		bool getName(std::string &name, const std::string &prevWord, const std::map<std::string, std::vector<std::string>> &allowedModifiers, std::map<std::string, std::string> &modifiers);

		void jump(unsigned long position);
		void back(unsigned long count = 1);
		void skip(unsigned long count = 1);
		bool skip(const std::string &string);
		bool skipChar(char c);
		void skipSpaces();
		void skipEmpty();
		void skipOrFail(const std::string &string, const std::string &error = "");
		void skipOrCancel(const std::string &string);
		
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


		bool expected(char expected, bool cancel = true, bool error = true, bool fatal = true);
		bool expected(const std::string &expected, bool cancel = true, bool error = true, bool fatal = true);

		void groupModifiers(const std::map<std::string, std::vector<std::string>> &allowedModifiers,
		                    const std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers,
		                    std::map<std::string, std::string> &outModifiers,
		                    std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers);
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
