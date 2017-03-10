#pragma once

namespace yaplc { namespace cemit {
	class Context {
	public:
		unsigned long indentation;
		char indentationChar;

	public:
		Context() :
			indentation(0),
			indentationChar(' ') {

		}
	};
} }
