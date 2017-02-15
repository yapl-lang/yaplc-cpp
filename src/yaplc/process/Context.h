#pragma once

#include <string>
#include <map>

namespace yaplc { namespace process {
	class Context {
	public:
		std::string package;
		std::string path;
		std::map<std::string, std::string> imports;
		std::map<std::string, std::string> names;
	
	public:
		Context() {
			
		}
		
		Context clone(const std::string &appender = "") {
			Context clone;
			
			clone.package = package;
			
			if (appender == "") {
				clone.path = path;
			} else {
				clone.path = path + "." + appender;
			}
			
			clone.imports = imports;
			clone.names = names;
			
			return clone;
		}
	};
} }
