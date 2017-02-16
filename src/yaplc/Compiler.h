#pragma once

#include <string>
#include <vector>
#include <map>
#include "CompilingError.h"

namespace yaplc {
	class Compiler {
	private:
		std::vector<CompilingError *> errors;
		std::map<std::string, std::string> files;

	public:
		Compiler();
		~Compiler();

		void file(const std::string &file);
		void project(const std::string &root);
	};
}
