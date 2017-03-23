#pragma once

#include <binstream/binstream.h>
#include <string>

namespace yaplc { namespace structure {
	class Node;

	class NodeFactory {
	public:
		static Node *create(const std::string &name);
		static Node *loadNode(const binstream &stream);
		static void loadNode(const binstream &stream, Node *node);
		static void saveNode(binstream &stream, const Node *node);
	};
} }
