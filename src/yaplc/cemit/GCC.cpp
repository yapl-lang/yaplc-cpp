#include "GCC.h"
#include "yaplc/util/replace.h"

namespace yaplc { namespace cemit {
	GCC::GCC() {
		reset();
	}

	GCC &GCC::includePath(const fs::path &path) {
		includePaths.push_back(path);

		return *this;
	}

	GCC &GCC::argument(const std::string &argument) {
		arguments.push_back(argument);

		return *this;
	}

	GCC &GCC::compile(const fs::path &file) {
		auto fileName = file.name();

		{
			auto pos = fileName.find_last_of('.');

			if (pos != std::string::npos) {
				fileName = fileName.substr(0, pos);
			}
		}

		fs::path object = file.parent()/(fileName + ".o");
		return compile(file, object);
	}

	GCC &GCC::compile(const fs::path &file, const fs::path &object) {
		files.emplace(file, object);

		return *this;
	}

	GCC &GCC::build(const fs::path &out) {
		std::string includePathString;
		{
			std::stringstream ss;

			for (auto path : includePaths) {
				ss << "-I\"" << util::replace(fs::escape(file.source), "$", "\\$") << "\" ";
			}

			includePathString = ss.str();
		}

		files.clear();

		return *this;
	}

	GCC &GCC::reset() {
		includePaths.clear();
		arguments.clear();
		files.clear();

		return *this;
	}
} }
