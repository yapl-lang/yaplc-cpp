#include "Emitter.h"
#include "LogicalError.h"
#include "yaplc/structure/PackageNode.h"
#include "yaplc/util/getlineandcolumn.h"
#include <functional>

namespace yaplc { namespace emit {
	Emitter::Emitter() {

	}

	Emitter::~Emitter() {

	}

	void Emitter::addObject(structure::RootNode *rootNode) {
		objects.push_back(rootNode);

		std::function<void(structure::TypeNode *)> processType = [&](structure::TypeNode *typeNode) {
			for (auto node : *typeNode) {
				if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
					processType(typeNode);
				}
			}

			types[typeNode->name->type] = typeNode;
		};

		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				for (auto node : *packageNode) {
					if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
						processType(typeNode);
					}
				}
			}
		}
	}

	structure::TypeNode *Emitter::getType(const std::string &fullPath) {
		return types[fullPath];
	}

	void Emitter::startEmit(const std::string &code, std::vector<CompilingError *> &errors, const structure::RootNode *rootNode) {
		this->code = &code;
		this->errors = &errors;

		emit(rootNode);
	}

	void Emitter::error(const std::string &message) {
		error(message, nullptr);
	}

	void Emitter::error(const std::string &message, structure::Node *node) {
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
