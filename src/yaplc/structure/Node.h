#pragma once

#include "yaplc/util/leftpad.h"
#include <string>
#include <sstream>

#define NODE_PROPS(PROPS) protected: virtual void showProps(std::ostream &stream, unsigned long indent = 0) const { stream << "(" << std::endl; PROPS util::leftpad(stream, indent) << ")"; }
#define NODE_PROPS_PARENT(PROPS, PARENT) protected: virtual void showProps(std::ostream &stream, unsigned long indent = 0) const { PARENT::showProps(stream, indent); stream << "(" << std::endl; PROPS util::leftpad(stream, indent) << ")"; }
#define NODE_PROP(prop, code) util::leftpad(stream, indent + 1) << #prop << " => "; code; stream << std::endl;
#define NODE_PROP_AUTO(prop) NODE_PROP(prop, showProp(stream, indent, prop));

namespace yaplc { namespace structure {
	class Childable;
	class Listable;
	
	class Node {
		friend class Childable;
		friend class Listable;
	private:
		std::string name;
		Childable *childableParent;
		Listable *listableParent;

	public:
		Node();
		Node(const std::string &name);
		Node(const std::string &name, Childable *parent);
		Node(Listable *parent);
		virtual ~Node();

		inline std::string getName() const {
			return name;
		}

		void setName(const std::string &name);
		
		inline Childable *getChildableParent() const {
			return childableParent;
		}
		
		inline Listable *getListableParent() const {
			return listableParent;
		}
		
		inline std::string show(unsigned long indent = 0) const {
			std::stringstream stream;
			show(stream, indent);
			return stream.str();
		}
		
		virtual void show(std::ostream &stream, unsigned long indent = 0) const;
	
	protected:
		virtual void showProps(std::ostream &stream, unsigned long indent = 0) const {  }

		inline void showProp(std::ostream &stream, unsigned long indent, Node *node) const {
			if (node == nullptr) {
				stream << "null";
			} else {
				node->show(stream, indent + 2);
			}
		}
	};
} }
