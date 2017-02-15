#include "Processor.h"

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
			}
		}
	}
	
	void Processor::process(structure::ImportNode *importNode, Configuration &configuration) {
		
	}
} }
