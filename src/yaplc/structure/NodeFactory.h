#pragma once

#include "binstream/stream.h"
#include <string>

namespace yaplc { namespace structure {
	class Node;

	class NodeFactory {
	public:
		static Node *create(const std::string &name);
		static Node *loadNode(const binstream::stream &stream);
		static void loadNode(const binstream::stream &stream, Node *node);
		static void saveNode(binstream::stream &stream, const Node *node);
	};
} }
