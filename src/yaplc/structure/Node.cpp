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
		containerParent(nullptr),
		begin(0), end(0) {

	}

	Node::Node(const std::string &name) :
		name(name),
		childableParent(nullptr),
		listableParent(nullptr),
		containerParent(nullptr),
		begin(0), end(0)  {

	}
	
	Node::Node(const std::string &name, Childable *parent) :
		name(name),
		childableParent(parent),
		listableParent(nullptr),
		containerParent(nullptr),
		begin(0), end(0)  {
		parent->add(this);
	}
	
	Node::Node(Listable *parent) :
		name(""),
		childableParent(nullptr),
		listableParent(parent),
		containerParent(nullptr),
		begin(0), end(0)  {
		parent->add(this);
	}
	
	Node::Node(Container *parent) :
		name(""),
		childableParent(nullptr),
		listableParent(nullptr),
		containerParent(parent),
		begin(0), end(0)  {
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

	std::string Node::getTypeName() const {
		int status;
		char *fullName = abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);

		std::vector<std::string> caps;
		regex::match("([A-Za-z0-9]*)$", fullName, caps, 1);
		auto name = caps[0];

		delete[] fullName;

		return name;
	}

	std::string Node::getShortName() const {
		auto name = getTypeName();

		if (name.substr(name.length() - 4) == "Node") {
			name = name.substr(0, name.length() - 4);
		}

		return name;
	}
	
	void Node::show(std::ostream &stream, unsigned long indent) const {
		stream << getShortName();
		
		showProps(stream, indent);
	}

	void Node::load(const binstream::stream &stream) {
		stream.getString(name);
		stream.get(begin);
		stream.get(end);
	}

	void Node::save(binstream::stream &stream) const {
		stream.putString(name);
		stream.put(begin);
		stream.put(end);
	}
} }
