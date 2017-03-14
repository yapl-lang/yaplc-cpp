#include "CodeStream.h"

namespace yaplc { namespace cemit {
	CodeStream::CodeStream() {
		reset();
	}

	CodeStream::~CodeStream() {

	}

	CodeStream &CodeStream::header(const std::string &header) {
		context.header = header;

		return *this;
	}

	CodeStream &CodeStream::includeGuard(const std::string &id) {
		context.includeGuard = id;

		return *this;
	}

	CodeStream &CodeStream::structPredeclaration(const std::string &name) {
		context.structPredeclaration = name;

		return *this;
	}

	CodeStream &CodeStream::include(const std::string &path, bool global) {
		for (const auto &entry : includeEntries) {
			if (path == entry.path) {
				return *this;
			}
		}

		includeEntries.push_back({path, global});

		return *this;
	}

	CodeStream &CodeStream::newline() {
		codeEntries.push_back({CodeEntry::Type::NewLine});

		return *this;
	}

	CodeStream &CodeStream::openBlock() {
		codeEntries.push_back({CodeEntry::Type::OpenBlock});

		return *this;
	}

	CodeStream &CodeStream::closeBlock() {
		codeEntries.push_back({CodeEntry::Type::CloseBlock});

		return *this;
	}

	CodeStream &CodeStream::push(const std::string &text) {
		codeEntries.push_back({CodeEntry::Type::Text, text});

		return *this;
	}

	void CodeStream::indent(std::ostream &stream) {
		for (unsigned long i = 0; i < context.indent; ++i) {
			stream << config.indentation;
		}
	}

	CodeStream &CodeStream::write(std::ostream &stream) {
		if (context.header != "") {
			stream << context.header << config.lineDelimiter;
		}

		if (context.includeGuard != "") {
			stream << "#ifndef " << context.includeGuard << config.lineDelimiter
				<< "#define " << context.includeGuard << config.lineDelimiter
				<< config.lineDelimiter;
		}

		if (context.structPredeclaration != "") {
			stream << "struct " << context.structPredeclaration << ";" << config.lineDelimiter
				<< config.lineDelimiter;
		}

		for (auto includeEntry : includeEntries) {
			stream << "#include ";

			if (includeEntry.global) {
				stream << "<";
			} else {
				stream << "\"";
			}

			stream << includeEntry.path;

			if (includeEntry.global) {
				stream << ">";
			} else {
				stream << "\"";
			}

			stream << config.lineDelimiter;
		}

		stream << config.lineDelimiter;

		bool wasNewLine = false;
		for (auto codeEntry : codeEntries) {
			switch (codeEntry.type) {
			case CodeEntry::Type::NewLine:
				wasNewLine = true;

				stream << config.lineDelimiter;
				break;
			case CodeEntry::Type::OpenBlock:
				if (wasNewLine) {
					indent(stream);
				}

				stream << "{";
				++context.indent;
				break;
			case CodeEntry::Type::CloseBlock:
				--context.indent;

				if (wasNewLine) {
					indent(stream);
				}

				stream << "}";
				break;
			case CodeEntry::Type::Text:
				if (wasNewLine) {
					indent(stream);
				}

				stream << codeEntry.value;
				break;
			}

			if (codeEntry.type != CodeEntry::Type::NewLine) {
				wasNewLine = false;
			}
		}

		if (context.includeGuard != "") {
			stream
				<< config.lineDelimiter
				<< "#endif " << config.lineDelimiter;
		}

		return *this;
	}

	CodeStream &CodeStream::write(CodeBackup &backup) {
		backup.includeEntries = includeEntries;
		backup.codeEntries = codeEntries;

		return *this;
	}

	CodeStream &CodeStream::reset() {
		config.lineDelimiter = "\n";
		config.indentation = "\t";

		includeEntries.clear();
		codeEntries.clear();


		context.indent = 0;

		context.header = "";
		context.includeGuard = "";

		context.structPredeclaration = "";

		return *this;
	}
} }