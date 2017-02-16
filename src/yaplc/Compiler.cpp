#include "Compiler.h"
#include "FatalError.h"

namespace yaplc {
	Compiler::Compiler() {

	}

	Compiler::~Compiler() {
		for (auto error : errors) {
			delete error;
		}
	}

	void Compiler::file(const std::string &file) {

	}

	void Compiler::project(const std::string &root) {
		/*auto rootPath = filesystem::path(root);

		if (!rootPath.exists()) {
			errors.push_back(new FatalError("Project root path '" + root + "' does not exists."));

			return;
		}

		auto srcPath = rootPath/"src";

		if (!srcPath.exists()) {
			errors.push_back(new FatalError("Project src path '" + root + "' does not exists."));

			return;
		}

		void include_directory(const filesystem::path &directory) {
			directory.
		}*/
	}
}
