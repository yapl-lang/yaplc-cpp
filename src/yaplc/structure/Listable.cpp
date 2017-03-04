#include "Listable.h"
#include "Childable.h"
#include "Container.h"
#include "yaplc/util/leftpad.h"
#include "NodeFactory.h"
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
	
	Listable::Listable(Container *parent) :
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
		} else if (child->containerParent != nullptr) {
			child->containerParent->set(nullptr);
			child->containerParent = nullptr;
		}
		
		child->listableParent = this;
		children.push_back(child);
	}
	
	void Listable::remove(Node *child) {
		if (child->listableParent == this) {
			children.erase(std::remove(children.begin(), children.end(), child));
		}
	}

	void Listable::clear() {
		for (auto child : children) {
			delete child;
		}

		children.clear();
	}

	void Listable::show(std::ostream &stream, unsigned long indent) const {
		Node::show(stream, indent);
		
		stream << " [" << std::endl;

		auto it = children.begin();
		auto end = children.end();

		if (it != end) {
			auto item = *it;
			util::leftpad(stream, indent + 1);
			item->show(stream, indent + 1);
			++it;

			for (; it != end; ++it) {
				auto item = *it;

				stream << ", ";
				item->show(stream, indent + 1);
			}
		}

		stream << std::endl;
		util::leftpad(stream, indent) << "]";
	}

	void Listable::load(const binstream::stream &stream) {
		Node::load(stream);

		unsigned long count;
		stream.get(count);

		while (count-- != 0) {
			if (auto node = NodeFactory::loadNode(stream)) {
				add(node);
			}
		}
	}

	void Listable::save(binstream::stream &stream) const {
		Node::save(stream);

		stream.put((unsigned long)children.size());
		for (auto child : children) {
			NodeFactory::saveNode(stream, child);
		}
	}
} }
