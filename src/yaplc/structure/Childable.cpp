#include "Childable.h"
#include "Listable.h"
#include "yaplc/util/leftpad.h"
#include <typeinfo>

namespace yaplc { namespace structure {
	Childable::Childable() :
		Node(),
		children() {
		
	}

	Childable::Childable(const std::string &name) :
		Node(name) {

	}
	
	Childable::Childable(const std::string &name, Childable *parent) :
		Node(name, parent),
		children() {
	}
	
	Childable::Childable(Listable *parent) :
		Node(parent),
		children() {
	}
	
	Childable::~Childable() {
		for (auto pair : children) {
			delete pair.second;
		}
	}
	
	void Childable::add(Node *child) {
		remove(name);

		if (child->childableParent != nullptr) {
			child->childableParent->remove(child);
		} else if (child->listableParent != nullptr) {
			child->listableParent->remove(child);
			child->listableParent = nullptr;
		}
		
		child->childableParent = this;
		children.push_back({child->name, child});
	}
	
	void Childable::add(const std::string &name, Node *child) {
		child->name = name;
		add(child);
	}
	
	void Childable::remove(Node *child) {
		for (auto it = begin(); it != end(); ++it) {
			if (it->second == child) {
				children.erase(it);

				break;
			}
		}
	}
	
	void Childable::remove(const std::string &name) {
		for (auto it = begin(); it != end(); ++it) {
			if (it->first == name) {
				children.erase(it);

				break;
			}
		}
	}
	
	void Childable::show(std::ostream &stream, unsigned long indent) const {
		Node::show(stream, indent);
		
		stream << " {" << std::endl;
		
		for (auto pair : children) {
			util::leftpad(stream, indent + 1) << pair.first << " => ";
			pair.second->show(stream, indent + 1);
			stream << std::endl;
		}
		
		util::leftpad(stream, indent) << "}";
	}
} }
