#include "Container.h"
#include "Childable.h"
#include "Listable.h"
#include "yaplc/util/leftpad.h"
#include <typeinfo>

namespace yaplc { namespace structure {
	Container::Container() :
		Node(),
		child(nullptr) {
		
	}

	Container::Container(const std::string &name) :
		Node(name),
		child(nullptr) {

	}

	Container::Container(const std::string &name, Childable *parent) :
		Node(name, parent),
		child(nullptr) {

	}

	Container::Container(Listable *parent) :
		Node(parent),
		child(nullptr) {

	}
	
	Container::Container(Container *parent) :
		Node(parent),
		child(nullptr) {
		parent->set(this);
	}
	
	Container::~Container() {
		delete child;
	}
	
	Node *Container::get() const {
		return child;
	}
	
	Node *Container::set(Node *child) {
		if (child->childableParent != nullptr) {
			child->childableParent->remove(child);
			child->childableParent = nullptr;
		} else if (child->listableParent != nullptr) {
			child->listableParent->remove(child);
		} else if (child->containerParent != nullptr) {
			child->containerParent->set(nullptr);
		}
		
		delete this->child;
		
		return this->child = child;
	}

	void Container::clear() {
		delete child;
		child = nullptr;
	}
	
	void Container::show(std::ostream &stream, unsigned long indent) const {
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
