#pragma once

#include "Node.h"
#include <vector>

namespace yaplc { namespace structure {
	class Listable : public Node {
	private:
		std::vector<Node *> children;

	public:
		Listable();
		Listable(const std::string &name);
		Listable(const std::string &name, Childable *parent);
		Listable(Listable *parent);
		Listable(Container *container);
		virtual ~Listable();
		
		void add(Node *child);
		void remove(Node *child);
		virtual void clear();

		inline unsigned long size() const {
			return children.size();
		}

		inline std::vector<Node *>::const_iterator begin() const {
			return children.begin();
		}
		
		inline std::vector<Node *>::const_iterator end() const {
			return children.end();
		}
		
		virtual void show(std::ostream &stream, unsigned long indent = 0) const;

	public:
		virtual void load(const binstream::stream &stream);
		virtual void save(binstream::stream &stream) const;
	};
} }
