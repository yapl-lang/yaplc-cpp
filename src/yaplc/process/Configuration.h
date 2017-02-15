#pragma once

#include <string>
#include <map>

namespace yaplc { namespace process {
	class Configuration {
	public:
		std::string package;
		std::map<std::string, std::string> imports;
	
	public:
		Configuration() {
			
		}
		
		Configuration clone() {
			return *this;
		}
	};
} }
