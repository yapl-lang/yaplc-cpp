#pragma once

#include <string>

namespace yaplc {
	class CompilingError {
	public:
		enum class Type { Syntax, Fatal };
	
	public:
		Type type;
		std::string message;
		
	public:
		CompilingError(Type type,
			std::string message);
		
	private:
		virtual void poly() const {  }
	};
}
