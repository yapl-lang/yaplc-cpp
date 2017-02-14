#include "Node.h"
#include "Childable.h"
#include "Listable.h"
#include "Container.h"
#include "regex/regex.h"
#include <typeinfo>
#include <cxxabi.h>

namespace yaplc { namespace structure {
	Node::Node() :
		name(""),
		childableParent(nullptr),
		listableParent(nullptr),
		containerParent(nullptr) {
		
	}

	Node::Node(const std::string &name) :
		name(name),
		childableParent(nullptr),
		listableParent(nullptr),
		containerParent(nullptr) {

	}
	
	Node::Node(const std::string &name, Childable *parent) :
		name(name),
		childableParent(parent),
		listableParent(nullptr),
		containerParent(nullptr) {
		parent->add(this);
	}
	
	Node::Node(Listable *parent) :
		name(""),
		childableParent(nullptr),
		listableParent(parent),
		containerParent(nullptr) {
		parent->add(this);
	}
	
	Node::Node(Container *parent) :
		name(""),
		childableParent(nullptr),
		listableParent(nullptr),
		containerParent(parent) {
		parent->set(this);
	}
	
	Node::~Node() {
		
	}

	void Node::setName(const std::string &name) {
		if ((this->name != name) && (childableParent != nullptr)) {
			childableParent->add(name, this);
		} else {
			this->name = name;
		}
	}
	
	void Node::show(std::ostream &stream, unsigned long indent) const {
		int status;
		char *fullName = abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);
		
		std::vector<std::string> caps;
		regex::match("([A-Za-z0-9]*)$", fullName, caps, 1);
		auto name = caps[0];
		
		if (name.substr(name.length() - 4) == "Node") {
			stream << name.substr(0, name.length() - 4);
		} else {
			stream << name;
		}
		
		showProps(stream, indent);
		
		delete[] fullName;
	}
} }
