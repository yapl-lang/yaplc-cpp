#pragma once

#include "node.h"
#include <map>

namespace yaplc { namespace structure {
	class Container : public Node {
	private:
		Node *child;

	public:
		Container();
		Container(Node *child);
		virtual ~Container();
		
		Node *get() const;
		Node *set(Node *child);
		
		virtual void show(std::stringstream &stream, unsigned long indent = 0) const;
	};
} }
