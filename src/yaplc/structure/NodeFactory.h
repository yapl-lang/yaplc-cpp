#pragma once

#include "Node.h"
#include <map>
#include <string>
#include <functional>

namespace yaplc { namespace structure {
	class NodeFactory {
	public:
		static Node *create(const std::string &name);
		static Node *loadNode(const binstream::stream &stream);
		static void loadNode(const binstream::stream &stream, Node *node);
		static void saveNode(binstream::stream &stream, const Node *node);
	};
} }
