#pragma once

namespace yaplc { namespace cemit {
	class Context {
	public:
		unsigned long indentation;
		unsigned long indentationWidth;
		char indentationChar;

	public:
		Context() :
			indentation(0),
			indentationWidth(1),
			indentationChar('\t') {

		}

		inline Context &indent() {
			++indentation;
			return *this;
		}
	};
} }
