#pragma once

#include "yaplc/CompilingError.h"
#include "yaplc/structure/RootNode.h"
#include "yaplc/structure/TypeNode.h"
#include "fs/fs.h"
#include <vector>
#include <map>
#include <string>

namespace yaplc { namespace emit {
	class Emitter {
	protected:
		std::string const *code;
		std::vector<CompilingError *> *errors;


		std::vector<structure::RootNode *> objects;
		std::map<std::string, structure::TypeNode *> types;

	public:
		Emitter();
		virtual ~Emitter();

		void addObject(structure::RootNode *rootNode);
		structure::TypeNode *getType(const std::string &fullPath);

		void startEmit(const std::string &code, std::vector<CompilingError *> &errors, const structure::RootNode *rootNode);

	protected:
		virtual void emit(const structure::RootNode *rootNode) = 0;

	public:
		virtual void build() = 0;

	protected:
		void error(const std::string &message);
		void error(const std::string &message, const structure::Node *node);
	};
} }
