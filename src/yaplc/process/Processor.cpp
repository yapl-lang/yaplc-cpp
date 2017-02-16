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
				Context packageContext;
				process(packageNode, packageContext);
			}
		}
	}
	
	void Processor::process(structure::PackageNode *packageNode, Context &context) {
		context.path = context.package = packageNode->name;
		
		for (auto node : *packageNode) {
			if (auto importNode = dynamic_cast<structure::ImportNode *>(node)) {
				process(importNode, context);
			} else if (auto classNode = dynamic_cast<structure::ClassNode *>(node)) {
				process(classNode, context);
			}
		}
	}
	
	void Processor::process(structure::ImportNode *importNode, Context &context) {
		auto target = importNode->target;
		
		std::vector<std::string> caps;
		regex::match("([^\\.]*)$", target, caps, 1);
		
		auto source = caps[0];
		
		context.imports[source] = target;
	}
	
	void Processor::process(structure::ClassNode *classNode, Context &context) {
		context.addName(classNode->name->type);
		
		process(classNode->base, context);
		for (auto interface : classNode->interfaces) {
			process(interface, context);
		}
		
		auto context2 = context.clone();
		
		
	}
	
	void Processor::process(structure::TypeNameNode *typeNameNode, Context &context) {
		contexty.fullName(typeNameNode->type);
	}
} }
