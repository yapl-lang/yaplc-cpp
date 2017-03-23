#pragma once

#include <fs/fs.h>
#include <string>
#include <vector>
#include <map>

namespace yaplc { namespace cemit {
	class GCC {
	private:
		std::vector<fs::path> includePaths;
		std::vector<std::string> arguments;

		std::map<fs::path, fs::path> files;

	public:
		GCC();

		GCC &includePath(const fs::path &path);
		GCC &argument(const std::string &argument);
		GCC &compile(const fs::path &file);
		GCC &compile(const fs::path &file, const fs::path &object);
		GCC &build(const fs::path &out);

		GCC &reset();
	};
} }
