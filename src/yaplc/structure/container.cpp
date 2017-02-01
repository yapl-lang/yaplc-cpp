#include "container.h"
#include "yaplc/util/leftpad.h"
#include <typeinfo>

namespace yaplc { namespace structure {
	Container::Container() :
		Node(),
		child(nullptr) {
		
	}
	
	Container::Container(Node *child) :
		Node(),
		child(child) {
	}
	
	Container::~Container() {
		delete child;
	}
	
	Node *Container::get() const {
		return child;
	}
	
	Node *Container::set(Node *child) {
		delete this->child;
		
		return this->child = child;
	}
	
	void Container::show(std::stringstream &stream, unsigned long indent) const {
		Node::show(stream, indent);
		
		stream << " <" << std::endl;
		
		util::leftpad(stream, indent + 1);
		
		if (child == nullptr) {
			stream << "null";
		} else {
			child->show(stream, indent + 1);
		}
		
		stream << std::endl;
		
		util::leftpad(stream, indent) << ">";
	}
} }
