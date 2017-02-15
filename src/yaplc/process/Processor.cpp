#include "Processor.h"
#include "regex/regex.h"

namespace yaplc { namespace process {
	Processor::Processor() {
		
	}
	
	Processor::~Processor() {
		
	}
	
	void Processor::process(structure::RootNode *rootNode) {
		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				Configuration packageConfiguration;
				process(packageNode, packageConfiguration);
			}
		}
	}
	
	void Processor::process(structure::PackageNode *packageNode, Configuration &configuration) {
		configuration.package = packageNode->name;
		
		for (auto node : *packageNode) {
			if (auto importNode = dynamic_cast<structure::ImportNode *>(node)) {
				process(importNode, configuration);
			} else if (auto classNode = dynamic_cast<structure::ClassNode *>(node)) {
				process(classNode, configuration);
			}
		}
	}
	
	void Processor::process(structure::ImportNode *importNode, Configuration &configuration) {
		auto target = importNode->target;
		
		std::vector<std::string> caps;
		regex::match("([^\\.]*)$", target, caps, 1);
		
		auto source = caps[0];
		
		configuration.imports[source] = target;
	}
	
	void Processor::process(structure::ClassNode *classNode, Configuration &configuration) {
		process(classNode->base, configuration);
		for (auto interface : classNode->interfaces) {
			process(interface, configuration);
		}
	}
	
	void Processor::process(structure::TypeNameNode *classNode, Configuration &configuration) {
		
	}
} }
