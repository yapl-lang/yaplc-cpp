#pragma once

#include "yaplc/structure/RootNode.h"
#include "fs/fs.h"
#include <string>

namespace yaplc { namespace emit {
	class Emitter {
	public:
		Emitter();
		virtual ~Emitter();

		void emit(const structure::RootNode *rootNode, const fs::path &outPath);

	protected:
		virtual void emit(const fs::path &outPath, const structure::RootNode *rootNode) = 0;
	};
} }
