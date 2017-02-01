#include "node.h"
#include "childable.h"
#include "listable.h"
#include "regex/regex.h"
#include <typeinfo>
#include <cxxabi.h>

namespace yaplc { namespace structure {
	Node::Node() :
		name(""),
		childableParent(nullptr),
		listableParent(nullptr) {
		
	}
	
	Node::Node(const std::string &name, Childable *parent) :
		name(name),
		childableParent(parent),
		listableParent(nullptr) {
		parent->add(this);
	}
	
	Node::Node(Listable *parent) :
		name(""),
		childableParent(nullptr),
		listableParent(parent) {
		parent->add(this);
	}
	
	Node::~Node() {
		
	}
	
	void Node::show(std::stringstream &stream, unsigned long indent) const {
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
