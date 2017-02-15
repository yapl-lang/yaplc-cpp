#pragma once

#include "Configuration.h"
#include "yaplc/structure/RootNode.h"
#include "yaplc/structure/PackageNode.h"
#include "yaplc/structure/ImportNode.h"
#include "yaplc/structure/ClassNode.h"
#include "yaplc/structure/TypeNameNode.h"

namespace yaplc { namespace process {
	class Processor {
	public:
		Processor();
		~Processor();
		
		void process(structure::RootNode *rootNode);
		void process(structure::PackageNode *packageNode, Configuration &configuration);
		void process(structure::ImportNode *importNode, Configuration &configuration);
		void process(structure::ClassNode *classNode, Configuration &configuration);
		void process(structure::TypeNameNode *classNode, Configuration &configuration);
	};
} }
