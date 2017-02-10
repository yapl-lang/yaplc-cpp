#pragma once

#include "Node.h"
#include <vector>

namespace yaplc { namespace structure {
	class Childable : public Node {
	private:
		std::vector<std::pair<std::string, Node *>> children;

	public:
		Childable();
		Childable(const std::string &name);
		Childable(const std::string &name, Childable *parent);
		Childable(Listable *parent);
		virtual ~Childable();
		
		void add(Node *child);
		void add(const std::string &name, Node *child);
		
		void remove(Node *child);
		void remove(const std::string &name);
		
		inline std::vector<std::pair<std::string, Node *>>::const_iterator begin() const {
			return children.begin();
		}
		
		inline std::vector<std::pair<std::string, Node *>>::const_iterator end() const {
			return children.end();
		}
		
		virtual void show(std::ostream &stream, unsigned long indent = 0) const;
	};
} }
