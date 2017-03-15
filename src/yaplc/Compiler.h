#pragma once

#include "CompilingError.h"
#include "yaplc/structure/RootNode.h"
#include "yaplc/emit/Emitter.h"
#include "fs/fs.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace yaplc {
	class Compiler {
	private:
		struct CodeFile {
			fs::path sourceFile;
			std::string package, code;
			structure::RootNode *root;
		};

		struct ErrorEntry {
			const CodeFile *file;
			CompilingError *error;

			inline ErrorEntry(const CodeFile *file, CompilingError *error) : file(file), error(error) {}
		};

		std::vector<ErrorEntry> errors;
		std::vector<CodeFile> files;

		fs::path root;

		emit::Emitter *emitter;

	public:
		Compiler();
		~Compiler();

		void file(const std::string &file, const std::string &package = "");
		void code(const std::string &code, const std::string &package = "");
		void project(const std::string &root);

	private:
		void directory(const fs::path &dir, const std::string &subPath);

	public:
		void compile();
		void process();
		void emit();
		void build();

	public:
		bool ready();
		inline void reportErrors() {
			reportErrors(std::cout);
		}
		void reportErrors(std::ostream &stream);
	};
}
