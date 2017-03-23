#pragma once

#include "Node.h"
#include "OperatorNode.h"
#include "MethodMemberNode.h"

namespace yaplc { namespace structure {
	class OperatorMemberNode : public MethodMemberNode {
	public:
		OperatorNode::Type type;

	public:
		virtual void load(const binstream &stream) {
			MethodMemberNode::load(stream);

			stream.get((unsigned char &)type);
		}

		virtual void save(binstream &stream) const {
			MethodMemberNode::save(stream);

			stream.put((unsigned char)type);
		}
	};
} }
