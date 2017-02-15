#pragma once

#include "yaplc/util/leftpad.h"
#include <string>
#include <sstream>

#define NODE_PROPS(PROPS) protected: virtual void showProps(std::ostream &stream, unsigned long indent = 0, bool ending = true) const { stream << "(" << std::endl; PROPS; if (ending) { util::leftpad(stream, indent) << ")"; } }
#define NODE_PROPS_PARENT(PROPS, PARENT) protected: virtual void showProps(std::ostream &stream, unsigned long indent = 0, bool ending = true) const { PARENT::showProps(stream, indent, false); PROPS; if (ending) { util::leftpad(stream, indent) << ")"; } }
#define NODE_PROP(prop, code) util::leftpad(stream, indent + 1) << #prop << " => "; code; stream << std::endl;
#define NODE_PROP_AUTO(prop) NODE_PROP(prop, showProp(stream, indent, prop));

namespace yaplc { namespace structure {
	class Childable;
	class Listable;
	class Container;
	
	class Node {
		friend class Childable;
		friend class Listable;
		friend class Container;
	private:
		std::string name;
		Childable *childableParent;
		Listable *listableParent;
		Container *containerParent;
		
		unsigned long begin, end;

	public:
		Node();
		Node(const std::string &name);
		Node(const std::string &name, Childable *parent);
		Node(Listable *parent);
		Node(Container *parent);
		virtual ~Node();

		inline std::string getName() const {
			return name;
		}

		void setName(const std::string &name);

		virtual void clear() {}

		inline Childable *getChildableParent() const {
			return childableParent;
		}
		
		inline Listable *getListableParent() const {
			return listableParent;
		}
		
		inline Container *getContainerParent() const {
			return containerParent;
		}
		
		
		inline unsigned long getBegin() const {
			return begin;
		}
		
		inline void setBegin(unsigned long value) {
			begin = value;
		}
		
		inline unsigned long getEnd() const {
			return end;
		}
		
		inline void setEnd(unsigned long value) {
			end = value;
		}
		
		
		inline std::string show(unsigned long indent = 0) const {
			std::stringstream stream;
			show(stream, indent);
			return stream.str();
		}
		
		virtual void show(std::ostream &stream, unsigned long indent = 0) const;
	
	protected:
		virtual void showProps(std::ostream &stream, unsigned long indent = 0, bool ending = true) const {  }

		inline void showProp(std::ostream &stream, unsigned long indent, Node *node) const {
			if (node == nullptr) {
				stream << "null";
			} else {
				node->show(stream, indent + 1);
			}
		}
	};
} }
