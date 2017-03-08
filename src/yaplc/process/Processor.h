#pragma once

#include "Context.h"
#include "yaplc/structure/RootNode.h"
#include "yaplc/structure/PackageNode.h"
#include "yaplc/structure/ImportNode.h"
#include "yaplc/structure/ClassNode.h"
#include "yaplc/structure/TypeNameNode.h"
#include "yaplc/structure/MemberNode.h"
#include "yaplc/structure/VariableMemberNode.h"
#include "yaplc/structure/MethodMemberNode.h"
#include "yaplc/structure/TypeExpressionNode.h"
#include "yaplc/CompilingError.h"

namespace yaplc { namespace process {
	class Processor {
	private:
		std::string const *code;
		std::vector<CompilingError *> *errors;

		std::map<std::string, structure::TypeNode *> types;

		std::map<std::string, std::map<std::string, std::vector<structure::TypeNameNode *>>> templatesRequirements;
		std::vector<std::string> resolvedTemplates;

		std::vector<structure::Node *> processed;

	public:
		Processor();
		~Processor();

		void addObject(structure::RootNode *rootNode, bool resolved = false);
		bool resolveTemplates(std::vector<structure::RootNode *> &newRoots);

		void process(structure::RootNode *rootNode, const std::string &code, std::vector<CompilingError *> &errors);
		void process(structure::PackageNode *packageNode, Context &context);
		void process(structure::ImportNode *importNode, Context &context);
		void process(structure::TypeNode *typeNode, Context &context);
		void process(structure::ClassNode *classNode, Context &context);
		void process(structure::TypeNameNode *typeNameNode, Context &context);
		void process(structure::MemberNode *memberNode, Context &context);
		void process(structure::VariableMemberNode *variableMemberNode, Context &context);
		void process(structure::MethodMemberNode *methodMemberNode, Context &context);
		void process(structure::ExpressionNode *expressionNode, Context &context);
		void process(structure::ArgumentsNode *argumentsNode, Context &context);
		void process(structure::TypeExpressionNode *typeExpressionNode, Context &context);

		void error(const std::string &message);
		void error(const std::string &message, structure::Node *node);
	};
} }
