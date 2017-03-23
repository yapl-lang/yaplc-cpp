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
#include <regex>

namespace yaplc { namespace parser {
	class BaseParser {
		friend class ParserManager;
	protected:
		static std::regex WordRegex;
		static std::regex LowercaseWordRegex;
		static std::regex UppercaseWordRegex;
		static std::regex WhitespaceRegex;

		ParserManager *parserManager;
		Configuration configuration, comingConfiguration;
		std::vector<Configuration> stack;
	
	protected:
		/**
		 * @return the current context position
		 */
		inline unsigned long position() const {
			return configuration.position;
		}

		/**
		 * @return the current context code length
		 */
		inline unsigned long length() const {
			return configuration.code->size();
		}

	public:
		BaseParser();

	private:
		void setup(ParserManager *parserManager);
		
	protected:
		/**
		 * Runs <code>T</code> parser in the current context. If parser fails, repairs context back and returns false.
		 *
		 * @tparam T parser type
		 * @tparam Args arguments types
		 * @param args arguments
		 * @return true if success
		 */
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

	private:
		unsigned long nodeBegin;

	protected:
		/**
		 * Marks the node begin position from the current context.
		 */
		inline void begin() {
			nodeBegin = position();
		}

		/**
		 * Marks the node begin position from the <code>position</code>.
		 *
		 * @param position node begin position
		 */
		inline void begin(unsigned long position) {
			nodeBegin = position;
		}

		/**
		 * Marks the <code>node</code> end position from the current context.
		 *
		 * @param node to set the end position
		 */
		inline void end(structure::Node *node) {
			node->setBegin(nodeBegin);
			node->setEnd(position() - 1);
		}

		/**
		 * Marks the <code>node</code> end position from the <code>position}</code>.
		 *
		 * @param node to set the end position
		 * @param position
		 */
		inline void end(structure::Node *node, unsigned long position) {
			node->setBegin(nodeBegin);
			node->setEnd(position);
		}

		/**
		 * Checks is it EOF, and returns true if it is.
		 *
		 * @return True if EOF
		 */
		bool end();

		/**
		 * Returns current character.
		 *
		 * @return a current character
		 */
		char get();

		/**
		 * Returns and skips current character.
		 *
		 * @return a current character
		 */
		char getAndSkip();

		/**
		 * Gets next <code>count</code> characters and returns it.
		 *
		 * @param count of characters to get
		 * @return next <code>count</code> characters(not skips)
		 */
		std::string get(unsigned long count);

		/**
		 * Skips all next characters that matches to <code>pattern</code>.
		 *
		 * @param pattern regular expression pattern to get
		 * @return true if success
		 */
		bool get(const std::regex &pattern);

		/**
		 * Skips all next characters that matches to <code>pattern</code> and saves result to <code>match</code>.
		 *
		 * @param pattern regular expression pattern to get
		 * @param match result object
		 * @return true if success
		 */
		bool get(const std::regex &pattern, std::smatch &match);

		/**
		 * Skips all next characters that matches to <code>pattern</code> and saves result to <code>match</code>.
		 *
		 * @param pattern regular expression pattern to get
		 * @param match result object
		 * @param position position of match
		 * @return true if success
		 */
		bool get(const std::regex &pattern, std::smatch &match, std::pair<unsigned long, unsigned long> position);

		/**
		 * Skips all next matches to <code>pattern</code> and saves results to <code>results</code>.
		 *
		 * @note <code>results</code> will be cleared before the operation.
		 *
		 * @param pattern regular expression pattern to get
		 * @param results vector of smatch's
		 * @return count of <code>results</code>
		 */
		unsigned long get(const std::regex &pattern, std::vector<std::smatch> &results);

		/**
		 * Skips all next matches to <code>pattern</code> and saves results to <code>results</code>.
		 *
		 * @note <code>results</code> will be cleared before the operation.
		 *
		 * @param pattern regular expression pattern to get
		 * @param results vector of std::tuple's (smatch match, unsigned long begin, unsigned long end)
		 * @return count of <code>results</code>
		 */
		unsigned long get(const std::regex &pattern, std::vector<std::tuple<std::smatch, unsigned long, unsigned long>> &results);

		/**
		 * Returns and skips the next word.
		 *
		 * @note Word matches regular expression '[A-Za-z][A-Za-z0-9]*'.
		 * @see skipEmpty.
		 *
		 * @param word
		 * @return true if success
		 */
		bool getWord(std::string &word);

		/**
		 * Returns and skips the next lowercase word.
		 *
		 * @note Word matches regular expression '[a-z][a-z0-9]*'.
		 * @see skipEmpty.
		 *
		 * @param word result word.
		 * @return true if success.
		 */
		bool getLowercaseWord(std::string &word);

		/**
		 * Returns and skips the next uppercase word.
		 *
		 * @note Word matches regular expression '[A-Z][A-Z0-9]*'.
		 * @see skipEmpty.
		 *
		 * @param word result word.
		 * @return true if success.
		 */
		bool getUppercaseWord(std::string &word);

		/**
		 * Gets next words and skips it.
		 *
		 * @note <code>words</code> will be cleared before operation.
		 * @see getWord.
		 *
		 * @param words vector to store result words.
		 * @return count of <code>words</code>.
		 */
		unsigned long getWords(std::vector<std::string> &words);

		/**
		 * Parses modifiers before given word and sets the position after the word.
		 * Uses <code>allowedModifiers</code> to fill <code>outModifiers</code>.
		 * If parsed modifiers are not found on <code>allowedModifiers</code>, it will be added to <code>otherModifiers</code>.
		 *
		 * @param beforeWord word, modifiers will be before it
		 * @param allowedModifiers a map of allowed modifiers. Default modifier is first, if modifiers starts from '~', it can't be used, but if it's first, it will be default modifier
		 * @param outModifiers output of allowed modifiers
		 * @param otherModifiers output of other modifiers
		 * @return true if success
		 */
		bool getModifiers(const std::string &beforeWord,
		                  const std::map<std::string, std::vector<std::string>> &allowedModifiers,
		                  std::map<std::string, std::string> &outModifiers,
		                  std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers);

		/**
		 * Parses modifiers before any given word of <code>beforeWords</code> and sets the position after the word.
		 * Uses <code>allowedModifiers</code> to fill <code>outModifiers</code>.
		 * If parsed modifiers are not found on <code>allowedModifiers</code>, it will be added to <code>otherModifiers</code>.
		 *
		 * @param beforeWords a set of beforeWord words, modifiers will be before it
		 * @param beforeWord a current word, that will be after modifiers
		 * @param allowedModifiers a map of allowed modifiers. Default modifier is first, if modifiers starts from '~', it can't be used, but if it's first, it will be default modifier
		 * @param outModifiers output of allowed modifiers
		 * @param otherModifiers output of other modifiers
		 * @return true if success
		 */
		bool getModifiers(const std::vector<std::string> &beforeWords,
		                  std::string &beforeWord,
		                  const std::map<std::string, std::vector<std::string>> &allowedModifiers,
		                  std::map<std::string, std::string> &outModifiers,
		                  std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers);

		/**
		 * Sets the current context position to <code>position</code>.
		 *
		 * @param position target
		 */
		void jump(unsigned long position);

		/**
		 * Returns position back to <code>count</code> symbols.
		 *
		 * @param count of symbols
		 */
		void back(unsigned long count = 1);

		/**
		 * Skips <code>count</code> symbols.
		 *
		 * @param count of symbols
		 */
		void skip(unsigned long count = 1);

		/**
		 * Tries to skip <code>string</code> and returns true on success.
		 *
		 * @param string to skip
		 * @return true if success
		 */
		bool skip(const std::string &string);

		/**
		 * Tries to skip <code>c</code> and returns true on success.
		 *
		 * @param c to skip
		 * @return true if success
		 */
		bool skipChar(char c);

		/**
		 * Skips any whitespaces.
		 */
		void skipSpaces();

		/**
		 * Skips empty things like spaces, tabs and comments.
		 */
		void skipEmpty();

		/**
		 * Skips <code>string</code> or cancels fatal and throws <code>error</code>.
		 *
		 * @see cancelFatal
		 * @see error
		 *
		 * @param string to skip
		 * @param error to show
		 */
		void skipOrFail(const std::string &string, const std::string &error = "");

		/**
		 * Skips <code>string<code> or cancels.
		 *
		 * @param string to skip
		 */
		void skipOrCancel(const std::string &string);

		/**
		 * Saves the context, and repairs it on cancel.
		 */
		void push();

		/**
		 * Pushes the context to the stack.
		 */
		void save();

		/**
		 * Restores the context from the stack.
		 */
		void restore();

		/**
		 * Skips the context from the stack.
		 */
		void norestore();

		/**
		 * Cancels the parser.
		 *
		 * @return
		 */
		bool cancel();

		/**
		 * Cancels the parser if end.
		 *
		 * @see end
		 *
		 * @return
		 */
		bool cancelIfEnd();

		/**
		 * Cancels compilation of the file(fatal error).
		 *
		 * @return
		 */
		bool cancelFatal();

		/**
		 * Adds the error to the error list.
		 *
		 * @param message error message
		 * @return true
		 */
		bool error(const std::string &message);

		/**
		 * Adds the error to the error list.
		 *
		 * @param message error message
		 * @param position position of error
		 * @return true
		 */
		bool error(const std::string &message, unsigned long position);

		/**
		 * Adds the error to the error list.
		 *
		 * @param message error message
		 * @param begin beginning of error
		 * @param end ending of error
		 * @return true
		 */
		bool error(const std::string &message, unsigned long begin, unsigned long end);

		/**
		 * Checks the next character, and if it is not same with <code>expected</code>, does given operation.
		 *
		 * @param expected expected char
		 * @param cancel does cancel, if fail
		 * @param error does show error, if fail
		 * @param fatal does cancel fatal, if fail
		 *
		 * @see cancel
		 * @see cancelFatal
		 * @see error
		 *
		 * @return true
		 */
		bool expected(char expected, bool cancel = true, bool error = true, bool fatal = true);

		/**
		 * Checks the next string, and if it is not same with <code>expected</code>, does given operation.
		 *
		 * @param expected expected string
		 * @param cancel does cancel, if fail
		 * @param error does show error, if fail
		 * @param fatal does cancel fatal, if fail
		 *
		 * @see cancel
		 * @see cancelFatal
		 * @see error
		 *
		 * @return true
		 */
		bool expected(const std::string &expected, bool cancel = true, bool error = true, bool fatal = true);

		/**
		 * Groups list of given modifiers as <code>getModifiers</code>.
		 *
		 * @see getModifiers
		 *
		 * @param allowedModifiers a map of allowed modifiers. Default modifier is first, if modifiers starts from '~', it can't be used, but if it's first, it will be default modifier
		 * @param modifiers a list of input modifiers
		 * @param outModifiers outModifiers output of allowed modifiers
		 * @param otherModifiers otherModifiers output of other modifiers
		 */
		void groupModifiers(const std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers,
			const std::map<std::string, std::vector<std::string>> &allowedModifiers,
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
