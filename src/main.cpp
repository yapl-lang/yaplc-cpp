#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

#include <binstream/binstream.h>
#include <commandline/CommandLine.h>

#include "yaplc/Compiler.h"
#include "yaplc/NotReadyException.h"
#include "yaplc/structure/NodeFactory.h"

struct CommandEntry {
	char shortName;
	std::string name;

	std::string args;

	std::string excerpt;
};

static std::vector<CommandEntry> commands {
	{'h', "help", "", "Display help information and list of commands."},
	{'v', "version", "", "Display version information."},
	{'s', "show", "<file...>", "Prints specified object file."}
};

void show_help() {
	std::cout << "Usage: yaplc [options] <project>" << std::endl;

	std::cout << "Options:" << std::endl;
	for (const auto &command : commands) {
		std::string names;
		names.reserve(2 + command.name.size() +
			2 + 1 + 1 +
			1 + command.args.size());

		if (command.shortName != '\0') {
			names += '-';
			names += command.shortName;

			if (!command.name.empty()) {
				names += ", --";
				names += command.name;
			}
		} else {
			names += "--";
			names += command.name;
		}

		if (!command.args.empty()) {
			names += " " + command.args;
		}

		std::cout << "    " << std::setw(20) << std::left << names << " " << std::right << command.excerpt << std::endl;
	}
}

void show_version() {
	std::cout << "YAPL Compiler written in C++ version 1.0.0 ALPHA." << std::endl;
}

int main(int argc, char *argv[]) {
	/*std::sort(commands.begin(), commands.end(), [](const CommandEntry &a, const CommandEntry &b) -> bool {
		std::string sortA = a.name, sortB = b.name;

		if (sortA.empty()) {
			sortA = a.shortName;
		}

		if (sortB.empty()) {
			sortB = b.shortName;
		}

		return sortA > sortB;
	});*/

	commandline::CommandLine commandLine{argc, argv};

	std::string param;
	while (true) {
		if (commandLine.param("help", 'h')) {
			show_help();
			return 0;
		} else if (commandLine.param("version", 'v')) {
			show_version();
			return 0;
		} else if (commandLine.param("show", 's')) {
			bool hasShowed = false;

			std::string file;
			while (commandLine.next(file)) {
				hasShowed = true;

				auto path = fs::relative(file);
				if (!path.exists()) {
					std::cout << "File \"" << file << "\" does not exist." << std::endl;
					continue;
				}

				auto node = yaplc::structure::NodeFactory::loadNode(binstream{path.content()});
				if (node == nullptr) {
					std::cout << "File \"" << file << "\" is not a valid object file." << std::endl;
					continue;
				}

				std::cout << file << ":" << std::endl;
				node->show(std::cout);
			}

			if (!hasShowed) {
				show_help();
				return 1;
			}

			return 0;
		} else {
			break;
		}
	}

	std::string project;
	if (!commandLine.next(project)) {
		show_help();
		return 1;
	}

	if (commandLine.available()) {
		show_help();
		return 1;
	}

	yaplc::Compiler compiler;

	try {
		compiler.project(fs::relative(project).full_name());

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

		return 1;
	}

	return 0;
}
