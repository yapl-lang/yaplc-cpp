#pragma once

#include "Context.h"
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
		void process(structure::PackageNode *packageNode, Context &context);
		void process(structure::ImportNode *importNode, Context &context);
		void process(structure::ClassNode *classNode, Context &context);
		void process(structure::TypeNameNode *typeNameNode, Context &context);
	};
} }
