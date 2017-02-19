#include "Emitter.h"

namespace yaplc { namespace emit {
	Emitter::Emitter() {

	}

	Emitter::~Emitter() {

	}

	void Emitter::emit(const structure::RootNode *rootNode, const fs::path &outPath) {
		emit(outPath, rootNode);
	}
} }
