#pragma once

#include "node.h"
#include <vector>

namespace yaplc { namespace structure {
	class Listable : public Node {
	private:
		std::vector<Node *> children;

	public:
		Listable();
		Listable(const std::string &name, Childable *parent);
		Listable(Listable *parent);
		virtual ~Listable();
		
		void add(Node *child);
		void remove(Node *child);
		
		inline std::vector<Node *>::const_iterator begin() const {
			return children.begin();
		}
		
		inline std::vector<Node *>::const_iterator end() const {
			return children.end();
		}
		
		virtual void show(std::stringstream &stream, unsigned long indent = 0) const;
	};
} }
