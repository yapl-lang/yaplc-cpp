#pragma once

#include "Node.h"
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
		Container(Container *parent);
		virtual ~Container();
		
		Node *get() const;
		template<class T> inline T *get() const {
			return dynamic_cast<T *>(get());
		}

		Node *set(Node *child);
		virtual void clear();
		
		virtual void show(std::ostream &stream, unsigned long indent = 0) const;
	};
} }
