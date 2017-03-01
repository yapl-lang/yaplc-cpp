#include "Emitter.h"
#include "yaplc/structure/PackageNode.h"
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

	void Emitter::startEmit(const structure::RootNode *rootNode) {
		emit(rootNode);
	}
} }
