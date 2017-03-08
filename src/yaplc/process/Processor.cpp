#include "Processor.h"
#include "LogicalError.h"
#include "yaplc/util/getlineandcolumn.h"
#include "regex/regex.h"
#include <functional>

namespace yaplc { namespace process {
#define CHECK_PROCESSED(node) if (std::find(this->processed.begin(), this->processed.end(), node) != this->processed.end()) { \
		return; \
	} \
	\
	this->processed.push_back(node);

	Processor::Processor() {
		
	}
	
	Processor::~Processor() {
		
	}

	void Processor::addObject(structure::RootNode *rootNode, bool resolved) {
		std::function<void(structure::TypeNode *, const std::string &)> processType = [&](structure::TypeNode *typeNode, const std::string &path) {
			auto npath = path + "." + (resolved ? typeNode->name->hashName() : typeNode->name->type);

			for (auto node : *typeNode) {
				if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
					processType(typeNode, npath);
				}
			}

			types[npath] = typeNode;
		};

		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				for (auto node : *packageNode) {
					if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
						processType(typeNode, packageNode->name);
					}
				}
			}
		}
	}

	bool Processor::resolveTemplates(std::vector<structure::RootNode *> &newRoots) {
		newRoots.clear();

		if (templatesRequirements.size() == 0) {
			return false;
		}

		for (auto templateRequirements : templatesRequirements) {
			for (auto templateRequirement : templateRequirements.second) {
				if (std::find(resolvedTemplates.begin(), resolvedTemplates.end(), templateRequirement.first) != resolvedTemplates.end()) {
					continue;
				}

				resolvedTemplates.push_back(templateRequirement.first);

				if (auto realType = types[templateRequirements.first]) {
					structure::Node *rootNode = realType;
					structure::RootNode *root;

					while (!(root = dynamic_cast<structure::RootNode *>(rootNode))) {
						if (!(rootNode = rootNode->getParent())) {
							break;
						}
					}

					if (root == nullptr) {
						continue;
					}

					root = (structure::RootNode *)root->clone();


					addObject(root, true);
				}
			}
		}

		return newRoots.size() != 0;
	}
	
	void Processor::process(structure::RootNode *rootNode, const std::string &code, std::vector<CompilingError *> &errors) {
		this->code = &code;
		this->errors = &errors;

		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				Context packageContext(types);
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
		CHECK_PROCESSED(importNode);

		context.name(importNode->name, importNode->target);
	}

	void Processor::process(structure::TypeNode *typeNode, Context &context) {
		CHECK_PROCESSED(typeNode);

		auto context2 = context.clone();
		context2.pushPath(typeNode->name->type);
		typeNode->name->type = context.path + "." + typeNode->name->type;

		if (typeNode->name->templateArguments.size() == 0) {
			if (auto classNode = dynamic_cast<structure::ClassNode *>(typeNode)) {
				process(classNode, context2);
			}
		}
	}
	
	void Processor::process(structure::ClassNode *classNode, Context &context) {
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

		if (typeNameNode->dimens.size() != 0) {
			bool hasSize = false;

			for (auto dimen : typeNameNode->dimens) {
				if (dimen != nullptr) {
					hasSize = true;
					break;
				}
			}

			if (!hasSize) {
				auto dimens = typeNameNode->dimens.size();

				auto realNode = new structure::TypeNameNode();
				realNode->type = typeNameNode->type;
				realNode->templateArguments = typeNameNode->templateArguments;

				typeNameNode->type = "yapl.Array";
				typeNameNode->templateArguments.clear();
				typeNameNode->dimens.clear();

				for (unsigned long i = 1; i < dimens; ++i) {
					auto subNode = new structure::TypeNameNode();
					subNode->type = "yapl.Array";
					typeNameNode->templateArguments.push_back(subNode);
					typeNameNode = subNode;
				}

				typeNameNode->templateArguments.push_back(realNode);
			}
		}

		if (typeNameNode->templateArguments.size() != 0) {
			for (auto templateArgument : typeNameNode->templateArguments) {
				process(templateArgument, context);
			}

			auto &templateRequirements = templatesRequirements[typeNameNode->type];
			auto &templateRequirement = templateRequirements[typeNameNode->hashName()];

			if (templateRequirement.size() == 0) {
				for (auto templateArgument : typeNameNode->templateArguments) {
					templateRequirement.push_back(templateArgument);
				}
			}
		}
	}

	void Processor::process(structure::MemberNode *memberNode, Context &context) {
		memberNode->setName(context.path + "." + memberNode->getName());

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

			if (value != nullptr) {
				process(value, context);
			}
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
