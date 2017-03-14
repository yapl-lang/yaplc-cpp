#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace yaplc { namespace cemit {
	class CodeStream {
	public:
		static struct NewLineT {} NewLine;
		static struct OpenBlockT {} OpenBlock;
		static struct CloseBlockT {} CloseBlock;

		struct IncludeEntry {
			std::string path;
			bool global;
		};

		struct CodeEntry {
			enum class Type {NewLine, OpenBlock, CloseBlock, Text} type;
			std::string value;
		};

		struct CodeBackup {
			std::vector<IncludeEntry> includeEntries;
			std::vector<CodeEntry> codeEntries;
		};

	private:
		struct {
			std::string lineDelimiter;
			std::string indentation;
		} config;

		std::vector<IncludeEntry> includeEntries;
		std::vector<CodeEntry> codeEntries;


		struct {
			unsigned long indent;

			std::string header;
			std::string includeGuard;

			std::string structPredeclaration;
		} context;

	public:
		CodeStream();
		~CodeStream();

		CodeStream &header(const std::string &header);
		CodeStream &includeGuard(const std::string &id);
		CodeStream &structPredeclaration(const std::string &name);

		CodeStream &include(const std::string &path, bool global = false);

		CodeStream &newline();
		CodeStream &openBlock();
		CodeStream &closeBlock();

		CodeStream &push(const std::string &text);

		template<class T> inline CodeStream &operator <<(const T &value) {
			std::stringstream stream;
			stream << value;
			return push(stream.str());
		}

		template<class T = const CodeBackup &> inline CodeStream &operator <<(const CodeBackup &value) {
			for (auto includeEntry : value.includeEntries) {
				include(includeEntry.path, includeEntry.global);
			}
			codeEntries.insert(codeEntries.end(), value.codeEntries.begin(), value.codeEntries.end());

			return *this;
		}

		template<class T = NewLineT> inline CodeStream &operator <<(NewLineT value) {
			return newline();
		}

		template<class T = OpenBlockT> inline CodeStream &operator <<(OpenBlockT value) {
			return openBlock();
		}

		template<class T = CloseBlockT> inline CodeStream &operator <<(CloseBlockT value) {
			return closeBlock();
		}

	private:
		void indent(std::ostream &stream);

	public:
		CodeStream &write(std::ostream &stream);
		CodeStream &write(CodeBackup &backup);
		CodeStream &reset();
	};
} }