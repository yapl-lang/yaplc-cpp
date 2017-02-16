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
		
		bool fullName(std::string &name) {
			std::map<std::string, std::string>::const_iterator it;
			
			for (it = context.names.begin(); it != context.names.end(); ++it) {
				if (strcmp(it->first.c_str(), name.c_str()) == 0) { // Normal equals operator not works, wtf?
					break;
				}
			}
			
			if (it == context.names.end()) {
				for (it = context.imports.begin(); it != context.imports.end(); ++it) {
					if (strcmp(it->first.c_str(), name.c_str()) == 0) { // Normal equals operator not works, wtf?
						break;
					}
				}
			}
			
			if (it == context.imports.end()) {
				return;
			}
			
			name = *it;
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
