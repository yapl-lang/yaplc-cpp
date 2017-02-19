#pragma once

#include "yaplc/emit/Emitter.h"
#include "yaplc/structure/PackageNode.h"
#include "yaplc/structure/TypeNode.h"
#include "yaplc/structure/ClassNode.h"
#include <fstream>

namespace yaplc { namespace cemit {
	class CEmitter : public emit::Emitter {
	private:
		std::ofstream outh, outc;

	public:
		CEmitter();
		virtual ~CEmitter();

	protected:
		virtual void emit(const fs::path &outPath, const structure::RootNode *rootNode);
		void emit(const fs::path &outPath, const structure::PackageNode *packageNode);
		void emit(const fs::path &outPath, const structure::TypeNode *typeNode);
		void emit(const structure::ClassNode *classNode);
	};
} }
