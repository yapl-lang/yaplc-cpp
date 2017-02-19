#include "Processor.h"
#include "LogicalError.h"
#include "yaplc/util/getlineandcolumn.h"
#include "regex/regex.h"

namespace yaplc { namespace process {
	Processor::Processor() {
		
	}
	
	Processor::~Processor() {
		
	}
	
	void Processor::process(structure::RootNode *rootNode, const std::string &code, std::vector<CompilingError *> &errors) {
		this->code = &code;
		this->errors = &errors;

		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				Context packageContext;
				process(packageNode, packageContext);
			}
		}
	}
	
	void Processor::process(structure::PackageNode *packageNode, Context &context) {
		context.path = context.package = packageNode->name;
		
		for (auto node : *packageNode) {
			if (auto importNode = dynamic_cast<structure::ImportNode *>(node)) {
				process(importNode, context);
			} else if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
				process(typeNode, context);
			}
		}
	}
	
	void Processor::process(structure::ImportNode *importNode, Context &context) {
		auto target = importNode->target;
		
		std::vector<std::string> caps;
		regex::match("([A-Za-z0-9]*)$", target, caps, 1);
		
		auto source = caps[0];
		
		context.imports[source] = target;
	}

	void Processor::process(structure::TypeNode *typeNode, Context &context) {
		if (auto classNode = dynamic_cast<structure::ClassNode *>(typeNode)) {
			auto context2 = context.clone();
			process(classNode, context2);
		}
	}
	
	void Processor::process(structure::ClassNode *classNode, Context &context) {
		context.pushPath(classNode->name->type);
		
		process(classNode->base, context);
		for (auto interface : classNode->interfaces) {
			process(interface, context);
		}
		
		for (auto member : *classNode) {
			if (auto typeNode = dynamic_cast<structure::TypeNode *>(member)) {
				process(typeNode, context);
			} else if (auto memberNode = dynamic_cast<structure::MemberNode *>(member)) {
				process(memberNode, context);
			}
		}
	}
	
	void Processor::process(structure::TypeNameNode *typeNameNode, Context &context) {
		context.fullName(typeNameNode->type);
	}

	void Processor::process(structure::MemberNode *memberNode, Context &context) {
		process(memberNode->type, context);

		if (auto variableMemberNode = memberNode->get<structure::VariableMemberNode>()) {
			process(variableMemberNode, context);
		} else if (auto methodMemberNode = memberNode->get<structure::MethodMemberNode>()) {
			process(methodMemberNode, context);
		}
	}

	void Processor::process(structure::VariableMemberNode *variableMemberNode, Context &context) {
		process(variableMemberNode->defaultValue, context);
	}

	void Processor::process(structure::MethodMemberNode *methodMemberNode, Context &context) {
		process(methodMemberNode->arguments, context);

		auto context2 = context.clone();
		process(methodMemberNode->body, context2);
	}

	void Processor::process(structure::ExpressionNode *expressionNode, Context &context) {
		for (auto expression : *expressionNode) {
			if (auto expressionNode = dynamic_cast<structure::ExpressionNode *>(expression)) {
				process(expressionNode, context);
			} else if (auto typeExpressionNode = dynamic_cast<structure::TypeExpressionNode *>(expression)) {
				process(typeExpressionNode, context);
			}
		}
	}

	void Processor::process(structure::ArgumentsNode *argumentsNode, Context &context) {
		for (auto argument : argumentsNode->arguments) {
			structure::TypeNameNode *type;
			std::string name;
			structure::ExpressionNode *value;

			std::tie(type, name, value) = argument;

			process(type, context);
			process(value, context);
		}
	}

	void Processor::process(structure::TypeExpressionNode *typeExpressionNode, Context &context) {
		process(typeExpressionNode->type, context);
	}


	void Processor::error(const std::string &message) {
		error(message, nullptr);
	}

	void Processor::error(const std::string &message, structure::Node *node) {
		unsigned long begin = 0, end = 0;

		if (node != nullptr) {
			begin = node->getBegin();
			end = node->getEnd();
		}

		unsigned long beginLine = 0, beginColumn = 0, endLine = 0, endColumn = 0;

		util::getlineandcolumn(*code, begin, beginLine, beginColumn);
		if (end != 0) {
			util::getlineandcolumn(*code, end, endLine, endColumn);
		}

		errors->push_back(new LogicalError(message, beginLine, beginColumn, endLine, endColumn));
	}
} }
