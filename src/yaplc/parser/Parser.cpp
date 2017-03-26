#include "Parser.h"
#include "SyntaxError.h"
#include "yaplc/util/getlineandcolumn.h"
#include "CommentsParser.h"
#include <algorithm>

namespace yaplc { namespace parser {
	std::regex BaseParser::WordRegex{"([A-Za-z][A-Za-z0-9]*)"};
	std::regex BaseParser::LowercaseWordRegex{"([a-z][a-z0-9]*)"};
	std::regex BaseParser::UppercaseWordRegex{"([A-Z][A-Z0-9]*)"};
	std::regex BaseParser::WhitespaceRegex{"\\s*"};

	BaseParser::BaseParser() :
		parserManager(nullptr),
		configuration() {
		
	}
	
	void BaseParser::setup(ParserManager *parserManager) {
		this->parserManager = parserManager;
	}
	
	bool BaseParser::end() {
		return configuration.position >= configuration.code->size();
	}
	
	char BaseParser::get() {
		if (configuration.position + 1 == configuration.code->size()) {
			cancel();
		}
		
		return configuration.code->operator[](configuration.position);
	}

	char BaseParser::getAndSkip() {
		char c = get();
		skip();
		return c;
	}
	
	std::string BaseParser::get(unsigned long count) {
		if (configuration.position + count >= configuration.code->size()) {
			cancel();
		}
		
		return configuration.code->substr(configuration.position, count);
	}

	bool BaseParser::get(const std::regex &pattern) {
		std::smatch match;
		return get(pattern, match);
	}

	bool BaseParser::get(const std::regex &pattern, std::smatch &match) {
		if (!std::regex_search(configuration.code->begin() + configuration.position, configuration.code->end(), match, pattern, std::regex_constants::match_continuous)) {
			return false;
		}

		if (match[0].length() == 0) {
			return false;
		}

		skip((unsigned long)match[0].length());

		return true;
	}

	bool BaseParser::get(const std::regex &pattern, std::smatch &match, std::pair<unsigned long, unsigned long> position) {
		position.first = this->position();
		auto result = get(pattern, match);
		position.second = this->position();

		return result;
	}

	unsigned long BaseParser::get(const std::regex &pattern, std::vector<std::smatch> &results) {
		results.clear();

		std::smatch match;
		while (get(pattern, match)) {
			results.push_back(match);
		}

		return results.size();
	}

	unsigned long BaseParser::get(const std::regex &pattern, std::vector<std::tuple<std::smatch, unsigned long, unsigned long>> &results) {
		results.clear();

		std::smatch match;
		std::pair<unsigned long, unsigned long> position;
		while (get(pattern, match, position)) {
			results.emplace_back(match, position.first, position.second);
		}

		return results.size();
	}

	bool BaseParser::getWord(std::string &word) {
		save();
		skipEmpty();

		std::smatch match;
		if (get(WordRegex, match)) {
			norestore();
			word = match[1];

			return true;
		}

		restore();
		return false;
	}
	
	bool BaseParser::getLowercaseWord(std::string &word) {
		save();
		skipEmpty();

		std::smatch match;
		if (get(LowercaseWordRegex, match)) {
			norestore();
			word = match[1];

			return true;
		}

		restore();
		return false;
	}

	bool BaseParser::getUppercaseWord(std::string &word) {
		save();
		skipEmpty();

		std::smatch match;
		if (get(UppercaseWordRegex, match)) {
			norestore();
			word = match[1];

			return true;
		}

		restore();
		return false;
	}

	unsigned long BaseParser::getWords(std::vector<std::string> &words) {
		words.clear();

		unsigned long count = 0;

		std::string word;
		while (getWord(word)) {
			words.push_back(word);
			++count;
		}

		return count;
	}
	
	bool BaseParser::getModifiers(const std::string &beforeWord,
	                              const std::map<std::string, std::vector<std::string>> &allowedModifiers,
	                              std::map<std::string, std::string> &outModifiers,
	                              std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers) {
		std::string beforeWordValue;
		return getModifiers(std::vector<std::string>{beforeWord}, beforeWordValue, allowedModifiers, outModifiers, otherModifiers);
	}
	
	bool BaseParser::getModifiers(const std::vector<std::string> &beforeWords,
	                              std::string &beforeWord,
	                              const std::map<std::string, std::vector<std::string>> &allowedModifiers,
	                              std::map<std::string, std::string> &outModifiers,
	                              std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers) {
		save();
		
		std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers;
		
		while (true) {
			std::string word;
			
			save();
			skipEmpty();
			auto pos1 = position();
			if (!getWord(word)) {
				restore();
				restore();
				return false;
			}
			auto pos2 = position();
			
			auto it = std::find(beforeWords.begin(), beforeWords.end(), word);
			if (it != beforeWords.end()) {
				restore();
				norestore();
				beforeWord = word;

				groupModifiers(modifiers, allowedModifiers, outModifiers,
					otherModifiers);
				return true;
			}
			
			modifiers.push_back(std::make_tuple(word, pos1, pos2 - 1));
		}
	}

	void BaseParser::jump(unsigned long position) {
		if (position > configuration.position) {
			cancel();
		}

		configuration.position = position;
	}

	void BaseParser::back(unsigned long count) {
		if (count > configuration.position) {
			cancel();
		}

		configuration.position -= count;
	}

	void BaseParser::skip(unsigned long count) {
		if (configuration.position + count >= configuration.code->size()) {
			cancel();
		}
		
		configuration.position += count;
	}
	
	bool BaseParser::skip(const std::string &string) {
		save();
		skipEmpty();
		
		if (get(string.size()) == string) {
			norestore();
			skip(string.size());
			return true;
		}
		
		restore();
		return false;
	}

	bool BaseParser::skipChar(char c) {
		skipEmpty();

		if (get() == c) {
			skip();

			return true;
		}

		return false;
	}
	
	void BaseParser::skipSpaces() {
		get(WhitespaceRegex);
	}
	
	void BaseParser::skipEmpty() {
		parse<CommentsParser>();
		skipSpaces();
	}
	
	void BaseParser::skipOrFail(const std::string &string, const std::string &error) {
		save();
		skipEmpty();
		
		try {
			if (get(string.size()) != string) {
				cancelFatal();
			}
		} catch (...) {
			restore();

			if (error != "") {
				this->error(error);
			}
			
			throw;
		}

		norestore();
		skip(string.size());
	}
	
	void BaseParser::skipOrCancel(const std::string &string) {
		skipEmpty();
		
		if (get(string.size()) != string) {
			cancel();
		}
		
		skip(string.size());
	}
	
	void BaseParser::push() {
		comingConfiguration = configuration;
	}
	
	void BaseParser::save() {
		stack.push_back(configuration);
	}
	
	void BaseParser::restore() {
		configuration = stack[stack.size() - 1];
		stack.pop_back();
	}
	
	void BaseParser::norestore() {
		stack.pop_back();
	}
	
	bool BaseParser::cancel() {
		throw CancelParseException();
	}
	
	bool BaseParser::cancelIfEnd() {
		skipEmpty();
		push(); // This function is used at parser start, so we can use this.
		
		if (end()) {
			cancel();
		}
		
		return true;
	}
	
	bool BaseParser::cancelFatal() {
		throw CancelFatalParseException();
	}
	
	bool BaseParser::error(const std::string &message) {
		return error(message, configuration.position);
	}
	
	bool BaseParser::error(const std::string &message, unsigned long position) {
		return error(message, position, 0);
	}
	
	bool BaseParser::error(const std::string &message, unsigned long begin, unsigned long end) {
		unsigned long beginLine = 0,
			beginColumn = 0,
			endLine = 0,
			endColumn = 0;

		util::getlineandcolumn(*configuration.code, begin, beginLine, beginColumn);
		if (end != 0) {
			util::getlineandcolumn(*configuration.code, end, endLine, endColumn);
		}
		
		parserManager->pushError(new SyntaxError(message, beginLine, beginColumn, endLine, endColumn));
		
		return true;
	}

	bool BaseParser::expected(char expected, bool pCancel, bool pError, bool fatal) {
		skipEmpty();
		if ((!end()) && (get() == expected)) {
			skip();

			return true;
		}

		if (pError) {
			error(std::string("Expected '") + expected + "'. Got '" + get() + "'.");
		}

		if (pCancel) {
			if (fatal) {
				cancelFatal();
			} else {
				cancel();
			}
		}
	}
	
	bool BaseParser::expected(const std::string &expected, bool pCancel, bool pError, bool fatal) {
		skipEmpty();
		
		if ((!end()) && (get(expected.size()) == expected)) {
			skip(expected.size());
			
			return false;
		}
		
		if (pError) {
			error(std::string("Expected '") + expected + "'.");
		}

		if (pCancel) {
			if (fatal) {
				cancelFatal();
			} else {
				cancel();
			}
		}
	}

	void BaseParser::groupModifiers(const std::vector<std::tuple<std::string, unsigned long, unsigned long>> modifiers,
		const std::map<std::string, std::vector<std::string>> &allowedModifiers,
		std::map<std::string, std::string> &outModifiers,
		std::vector<std::tuple<std::string, unsigned long, unsigned long>> &otherModifiers) {
		for (auto word : modifiers) {
			for (auto allowedModifierSet : allowedModifiers) {
				for (auto allowedModifier : allowedModifierSet.second) {
					if (std::get<0>(word) == allowedModifier) {
						auto it = outModifiers.begin();
						for (; it != outModifiers.end(); ++it) {
							if ((*it).first == allowedModifierSet.first) {
								break;
							}
						}

						if (it != outModifiers.end()) {
							auto word1 = std::get<0>(word);
							auto word2 = (*it).second;

							if (word1 == word2) {
								error("Modifier '" + word1 + "' is declared twice.", std::get<1>(word), std::get<2>(word));
							} else {
								error("'" + word1 + "' and '" + word2 + "' modifiers are incompatible.", std::get<1>(word), std::get<2>(word));
							}
						} else {
							outModifiers[allowedModifierSet.first] = std::get<0>(word);
						}

						goto next;
					}
				}
			}

			otherModifiers.push_back(word);

next:;
		}
	}
} }
