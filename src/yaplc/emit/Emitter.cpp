#include "Emitter.h"

namespace yaplc { namespace emit {
	Emitter::Emitter() {

	}

	Emitter::~Emitter() {

	}

	void Emitter::startEmit(const structure::RootNode *rootNode) {
		emit(rootNode);
	}
} }
