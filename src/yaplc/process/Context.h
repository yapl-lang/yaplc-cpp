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
		
		void pushPath(const std::string &name) {
			path += "." + name;
		}
		
		void name(const std::string &name) {
			
		}
		
		void fullName(std::string &name) {
			std::map<std::string, std::string>::const_iterator it;
			
			for (it = names.begin(); it != names.end(); ++it) {
				if (it->first == name) {
					break;
				}
			}
			
			if (it == names.end()) {
				for (it = imports.begin(); it != imports.end(); ++it) {
					if (it->first == name) {
						break;
					}
				}
			}
			
			if (it == imports.end()) {
				return;
			}
			
			name = it->second;
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
