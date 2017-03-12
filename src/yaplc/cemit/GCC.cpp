#include "GCC.h"
#include "yaplc/util/replace.h"
#include <sstream>

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
		std::string includePathsString;
		{
			std::stringstream ss;

			for (auto path : includePaths) {
				ss << "-I\"" << util::replace(fs::escape(path), "$", "\\$") << "\" ";
			}

			includePathsString = ss.str();
		}

		std::string argumentsString;
		{
			std::stringstream ss;

			for (auto argument : arguments) {
				ss << argument << " ";
			}

			argumentsString = ss.str();
		}

		std::stringstream buildss;
		buildss << "gcc ";

		for (auto file : files) {
			std::stringstream ss;

			buildss << "\"" << util::replace(fs::escape(file.second), "$", "\\$") << "\" ";

			ss << "gcc ";
			ss << includePathsString;
			ss << argumentsString;
			ss << "-c \"" << util::replace(fs::escape(file.first), "$", "\\$") << "\" ";
			ss << "-o \"" << util::replace(fs::escape(file.second), "$", "\\$") << "\" ";

			system(ss.str().c_str());
		}

		buildss << "-o \"" << util::replace(fs::escape(out), "$", "\\$") << "\"";
		system(buildss.str().c_str());

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
