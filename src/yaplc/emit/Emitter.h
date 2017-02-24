#pragma once

#include "yaplc/structure/RootNode.h"
#include "fs/fs.h"
#include <string>

namespace yaplc { namespace emit {
	class Emitter {
	public:
		Emitter();
		virtual ~Emitter();

		void startEmit(const structure::RootNode *rootNode);

	protected:
		virtual void emit(const structure::RootNode *rootNode) = 0;
	};
} }
