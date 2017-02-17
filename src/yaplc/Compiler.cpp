#include "Compiler.h"
#include "FatalError.h"
#include "PositionalError.h"
#include "NotReadyException.h"
#include "util/markline.h"
#include "yaplc/parser/ParserManager.h"

namespace yaplc {
	void showError(const std::string &code,
	               const CompilingError *error, std::ostream &out) {
		out << "[";

		switch (error->type) {
		case CompilingError::Type::Syntax:
			out << "Syntax";
			break;
		case CompilingError::Type::Logical:
			out << "Logical";
			break;
		case CompilingError::Type::Fatal:
			out << "Fatal";
			break;
		}

		out
			<< " Error]";

		if (auto positionalError = dynamic_cast<const PositionalError *>(error)) {
			if (positionalError->isDouble) {
				out
					<< "["
					<< positionalError->beginLineNumber + 1
					<< ":"
					<< positionalError->beginColumnNumber
					<< "-"
					<< positionalError->endLineNumber + 1
					<< ":"
					<< positionalError->endColumnNumber
					<< "]";
			} else {
				out
					<< "["
					<< positionalError->beginLineNumber + 1
					<< ":"
					<< positionalError->beginColumnNumber
					<< "]";
			}

			out
				<< ": "
				<< positionalError->message
				<< std::endl;

			if (positionalError->isDouble) {
				out
					<< util::markline(code,
						positionalError->beginLineNumber, positionalError->beginColumnNumber,
						positionalError->endLineNumber, positionalError->endColumnNumber)
					<< std::endl;
			} else {
				out
					<< util::markline(code,
						positionalError->beginLineNumber, positionalError->beginColumnNumber)
					<< std::endl;
			}
		} else {
			out
				<< ": "
				<< error->message
				<< std::endl;
		}
	}

	Compiler::Compiler() {

	}

	Compiler::~Compiler() {
		for (auto error : errors) {
			delete error.error;
		}
	}

	void Compiler::file(const std::string &file, const std::string &package) {
		code(fs::path(file).content(), package);
	}

	void Compiler::code(const std::string &code, const std::string &package) {
		files.push_back({package, code});
	}

	void Compiler::project(const std::string &root) {
		auto rootPath = this->root = fs::path(root);

		if (!rootPath.exists()) {
			errors.push_back({nullptr, new FatalError("Project root path '" + rootPath.full_name() + "' does not exists.")});

			return;
		}

		auto srcPath = rootPath/"src";

		if (!srcPath.exists()) {
			errors.push_back({nullptr, new FatalError("Project src path '" + srcPath.full_name() + "' does not exists.")});

			return;
		}

		directory(srcPath, "");
	}

	void Compiler::directory(const fs::path &dir, const std::string &subPath) {
		for (auto sub : dir.list()) {
			if (sub.is_directory()) {
				directory(sub, (subPath == "") ? (sub.name()) : (subPath + "." + sub.name()));
			} else if (sub.is_file()) {
				auto name = sub.name();

				if (name.length() > 5) {
					if (name.substr(name.length() - 4) != ".ypl") {
						continue;
					}

					auto appender = name.substr(0, name.length() - 4);

					code(sub.content(), (subPath == "") ? (appender) : (subPath + "." + appender));
				}
			}
		}
	}

	void Compiler::compile() {
		if (!ready()) {
			throw NotReadyException();
		}

		if (!root.exists()) {
			root = fs::temp();
		}

		parser::ParserManager parser;

		for (auto &file : files) {
			std::vector<CompilingError *> errors;

			file.root = parser.parse(file.code, errors);
			for (auto error : errors) {
				this->errors.push_back({&file, error});
			}
		}
	}

	bool Compiler::ready() {
		return errors.size() == 0;
	}

	void Compiler::reportErrors(std::ostream &stream) {
		for (auto error : errors) {
			showError(error.file->code, error.error, stream);
		}
	}
}
