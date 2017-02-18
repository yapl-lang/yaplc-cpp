#include "NodeFactory.h"
#include "ArgumentsNode.h"
#include "CharNode.h"
#include "Childable.h"
#include "ClassNode.h"
#include "Container.h"
#include "DeclareVariableNode.h"
#include "DoWhileOperatorNode.h"
#include "ExpressionNode.h"
#include "IfOperatorNode.h"
#include "ImportNode.h"
#include "Listable.h"
#include "MemberNode.h"
#include "MethodMemberNode.h"
#include "Node.h"
#include "NullNode.h"
#include "NumberNode.h"
#include "OperatorNode.h"
#include "PackageNode.h"
#include "RootNode.h"
#include "StringNode.h"
#include "TypeExpressionNode.h"
#include "TypeNameNode.h"
#include "TypeNode.h"
#include "VariableMemberNode.h"
#include "WhileOperatorNode.h"

namespace yaplc { namespace structure {
	std::map<std::string, std::function<Node *()>> nodes;

	Node *NodeFactory::create(const std::string &name) {
		auto it = nodes.find(name);

		if (it == nodes.end()) {
			return nullptr;
		}

		return it->second();
	}

	Node *NodeFactory::loadNode(const binstream::stream &stream) {
		std::string className;
		stream.getString(className);

		auto node = create(className);
		if (node == nullptr) {
			return nullptr;
		}

		node->load(stream);
		return node;
	}

	void NodeFactory::loadNode(const binstream::stream &stream, Node *node) {
		std::string className;
		stream.getString(className);

		if (node->getTypeName() != className) {
			throw std::runtime_error("Type name mismatch.");
		}

		node->load(stream);
	}

	void NodeFactory::saveNode(binstream::stream &stream, const Node *node) {
		stream.putString(node->getTypeName());
		node->save(stream);
	}

	void *add(const std::string &name, const std::function<Node *()> &creator) {
		nodes[name] = creator;
	}

#define REGISTER_NODE(CLASS_NAME) auto NOTHING_ ## CLASS_NAME = add(#CLASS_NAME , []() { return new CLASS_NAME(); });

	REGISTER_NODE(ArgumentsNode)
	REGISTER_NODE(CharNode)
	REGISTER_NODE(Childable)
	REGISTER_NODE(ClassNode)
	REGISTER_NODE(Container)
	REGISTER_NODE(DeclareVariableNode)
	REGISTER_NODE(DoWhileOperatorNode)
	REGISTER_NODE(ExpressionNode)
	REGISTER_NODE(IfOperatorNode)
	REGISTER_NODE(ImportNode)
	REGISTER_NODE(Listable)
	REGISTER_NODE(MemberNode)
	REGISTER_NODE(MethodMemberNode)
	REGISTER_NODE(Node)
	REGISTER_NODE(NullNode)
	REGISTER_NODE(OperatorNode)
	REGISTER_NODE(PackageNode)
	REGISTER_NODE(RootNode)
	REGISTER_NODE(StringNode)
	REGISTER_NODE(TypeExpressionNode)
	REGISTER_NODE(TypeNameNode)
	REGISTER_NODE(TypeNode)
	REGISTER_NODE(VariableMemberNode)
	REGISTER_NODE(WhileOperatorNode)
} }