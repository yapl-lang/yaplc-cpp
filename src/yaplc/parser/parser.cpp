#include "parser.h"
#include "syntaxerror.h"
#include "yaplc/util/getlineandcolumn.h"
#include "regex/regex.h"
#include "commentsparser.h"
#include <algorithm>

namespace yaplc { namespace parser {
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
	
	std::string BaseParser::get(unsigned long count) {
		if (configuration.position + count >= configuration.code->size()) {
			cancel();
		}
		
		return configuration.code->substr(configuration.position, count);
	}

	bool BaseParser::get(const std::string &pattern) {
		return get(pattern, {});
	}

	bool BaseParser::get(const std::string &pattern, const std::vector<std::string *> &caps) {
		std::vector<std::string> capsArray;

		bool result = regex::match("^(" + pattern + ")", (*configuration.code).substr(configuration.position), capsArray, caps.size() + 1);

		if (result) {
			configuration.position += capsArray[0].size();

			if (caps.size() != 0) {
				auto capSource = ++capsArray.begin();
				auto capTarget = caps.begin();

				for (; capSource != capsArray.end(); ++capSource, ++capTarget) {
					auto target = *capTarget;

					if (target != nullptr) {
						*target = *capSource;
					}
				}
			}
		}

		return result;
	}

	unsigned long BaseParser::get(const std::string &pattern, std::vector<std::string> &results) {
		unsigned long count = 0;
		std::string word;

		skipEmpty();
		while (get(pattern, {&word})) {
			results.push_back(word);
			skipEmpty();
			++count;
		}

		return count;
	}

	unsigned long BaseParser::get(const std::string &pattern, std::vector<std::pair<std::string, std::pair<unsigned long, unsigned long>>> &results) {
		unsigned long count = 0;
		std::string word;

		skipEmpty();
		while (get(pattern, {&word})) {
			results.push_back({word, {position() - word.length(), position() - 1}});
			skipEmpty();
			++count;
		}

		return count;
	}

	bool BaseParser::getWord(std::string &word) {
		skipEmpty();
		
		return get("([A-Za-z][A-Za-z0-9]*)", {&word});
	}
	
	bool BaseParser::getLowercaseWord(std::string &word) {
		skipEmpty();
		
		return get("([a-z][a-z0-9]*)", {&word});
	}

	unsigned long BaseParser::getWords(std::vector<std::string> &words) {
		unsigned long count = 0;

		std::string word;
		while (getWord(word)) {
			words.push_back(word);
			++count;
		}

		return count;
	}
	
	bool BaseParser::getModifiers(const std::map<std::string, std::vector<std::string>> &allowedModifiers, std::map<std::string, std::string> &modifiers) {
		save();
		
		while (true) {
			save();
			
			std::string word;
			if (!getWord(word)) {
				restore();
				break;
			}
			
			for (auto allowedModifierSet : allowedModifiers) {
				for (auto allowedModifier : allowedModifierSet.second) {
					if ((word == allowedModifier) || ("~" + word == allowedModifier)) {
						auto it = modifiers.find(allowedModifierSet.first);
						
						if (it != modifiers.end()) {
							error("'" + word + "' and '" + (*it).second + "' modifiers are incompatible.", position() - word.length(), position() - 1);
						} else {
							modifiers[allowedModifierSet.first] = word;
						}
						
						goto next;
					}
				}
			}
			
			restore();
			break;
			
next:
			norestore();
		}
		
		for (auto allowedModifier : allowedModifiers) {
			if (modifiers.find(allowedModifier.first) == modifiers.end()) {
				auto modifier = allowedModifier.second[0];
				
				if (modifier[0] == '~') {
					modifiers[allowedModifier.first] = modifier.substr(1);
				} else {
					modifiers[allowedModifier.first] = modifier;
				}
			}
		}
		
		norestore();
		
		return true;
	}
	
	bool BaseParser::getName(std::string &name, const std::string &prevWord) {
		std::map<std::string, std::vector<std::string>> allowedModifiers;
		std::map<std::string, std::string> modifiers;
		
		return getName(name, prevWord, allowedModifiers, modifiers);
	}
	
	bool BaseParser::getName(std::string &name, const std::string &prevWord, const std::map<std::string, std::vector<std::string>> &allowedModifiers, std::map<std::string, std::string> &modifiers) {
		std::string word;
		std::vector<std::pair<unsigned long, unsigned long>> otherModifiers;
		
		save();
		
		while (true) {
			if (!getWord(word)) {
				restore();
				return false;
			}
			
			if (word == prevWord) {
				if (!getWord(name)) {
					error("Name expected.");
					restore();
					return false;
				}
				
				for (auto otherModifier : otherModifiers) {
					error("Unknown modifier.", otherModifier.first, otherModifier.second);
				}
				
				break;
			}
			
			for (auto allowedModifierSet : allowedModifiers) {
				for (auto allowedModifier : allowedModifierSet.second) {
					if (word == allowedModifier) {
						auto it = modifiers.find(allowedModifierSet.first);
						
						if (it != modifiers.end()) {
							error("'" + word + "' and '" + (*it).second + "' modifiers are incompatible.", position() - word.length(), position() - 1);
						} else {
							modifiers[allowedModifierSet.first] = word;
						}
						
						goto next;
					}
				}
			}
			
			otherModifiers.push_back({position() - word.length(), position() - 1});
			
next:
			continue;
		}
		
		for (auto allowedModifier : allowedModifiers) {
			if (modifiers.find(allowedModifier.first) == modifiers.end()) {
				modifiers[allowedModifier.first] = allowedModifier.second[0];
			}
		}
		
		norestore();
		
		return true;
	}
	
	void BaseParser::skip(unsigned long count) {
		if (configuration.position + count >= configuration.code->size()) {
			cancel();
		}
		
		configuration.position += count;
	}
	
	void BaseParser::skipSpaces() {
		get("\\s*");
	}
	
	void BaseParser::skipEmpty() {
		parse<CommentsParser>();
		skipSpaces();
		push();
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
		push();
		
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
} }
