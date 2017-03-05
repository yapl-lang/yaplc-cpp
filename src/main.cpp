#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

#include "yaplc/Compiler.h"
#include "yaplc/NotReadyException.h"

int main(int argc, char **argv) {
	yaplc::Compiler compiler;

#ifdef INPUT_FILE
	std::string root(INPUT_FILE);
#else
	std::string root(argv[1]);
#endif

	try {
		//compiler.project(fs::relative(root).full_name());
		compiler.project(fs::relative(root + "/../yapl").full_name());

		std::cout << "Compiling..." << std::endl;
		compiler.compile();

		std::cout << "Processing..." << std::endl;
		compiler.process();

		std::cout << "Emitting..." << std::endl;
		compiler.emit();

		std::cout << "Building..." << std::endl;
		compiler.build();
	} catch (const yaplc::NotReadyException &) {
		compiler.reportErrors(std::cout);
	}

	return 0;
}
