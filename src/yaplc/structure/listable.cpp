#include "listable.h"
#include "childable.h"
#include "yaplc/util/leftpad.h"
#include <typeinfo>
#include <algorithm>

namespace yaplc { namespace structure {
	Listable::Listable() :
		Node(),
		children() {
		
	}

	Listable::Listable(const std::string &name) :
		Node(name),
		children() {
	}
	
	Listable::Listable(const std::string &name, Childable *parent) :
		Node(name, parent),
		children() {
	}
	
	Listable::Listable(Listable *parent) :
		Node(parent),
		children() {
	}
	
	Listable::~Listable() {
		for (auto child : children) {
			delete child;
		}
	}
	
	void Listable::add(Node *child) {
		if (child->childableParent != nullptr) {
			child->childableParent->remove(child);
			child->childableParent = nullptr;
		} else if (child->listableParent != nullptr) {
			child->listableParent->remove(child);
		}
		
		child->listableParent = this;
		children.push_back(child);
	}
	
	void Listable::remove(Node *child) {
		if (child->listableParent == this) {
			children.erase(std::remove(children.begin(), children.end(), child));
		}
	}
	
	void Listable::show(std::ostream &stream, unsigned long indent) const {
		Node::show(stream, indent);
		
		stream << " [" << std::endl;
		
		for (auto item : children) {
			util::leftpad(stream, indent + 1);
			item->show(stream, indent + 1);
			stream << std::endl;
		}
		
		util::leftpad(stream, indent) << "]";
	}
} }
