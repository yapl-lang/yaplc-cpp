#pragma once

#include "node.h"
#include <map>

namespace yaplc { namespace structure {
	class Container : public Node {
	private:
		Node *child;

	public:
		Container();
		Container(const std::string &name);
		Container(const std::string &name, Childable *parent);
		Container(Listable *parent);
		virtual ~Container();
		
		Node *get() const;
		Node *set(Node *child);
		
		virtual void show(std::stringstream &stream, unsigned long indent = 0) const;
	};
} }
