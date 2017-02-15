#pragma once

#include <string>
#include <map>

namespace yaplc { namespace process {
	class Context {
	public:
		std::string package;
		std::map<std::string, std::string> imports;
	
	public:
		Context() {
			
		}
		
		Context clone() {
			return *this;
		}
	};
} }
