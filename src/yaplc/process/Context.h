#pragma once

#include <string>
#include <vector>
#include <map>

namespace yaplc { namespace process {
	class Context {
	public:
		std::string package;
		std::string path;
		std::map<std::string, std::string> names;
		std::vector<std::string> const &types;
	
	public:
		Context(const std::vector<std::string> &types) : types(types) {
			name("Object", "yapl.Object");
			name("String", "yapl.String");
			name("object", "yapl.Object");
			name("string", "yapl.String");
		}
		
		void pushPath(const std::string &name) {
			path += "." + name;
		}
		
		void name(const std::string &name) {
			auto delim = name.find_last_of('.');
			
			if (delim == std::string::npos) {
				this->name(name, name);
			} else {
				auto ending = name.substr(delim + 1);

				if (ending == "*") {
					auto beginning = name.substr(0, delim);

					for (auto type : types) {
						if ((type.size() > beginning.size()) && (type.substr(0, beginning.size()) == beginning)) {
							this->name(type);
						}
					}
				} else {
					this->name(ending, name);
				}
			}
		}
		
		void name(const std::string &name, const std::string &fullName) {
			names[name] = fullName;
		}
		
		void fullName(std::string &name) {
			std::string beginning;
			
			auto pos = name.find(".");
			if (pos == std::string::npos) {
				beginning = name;
			} else {
				beginning = name.substr(0, pos);
			}
			
			std::map<std::string, std::string>::const_iterator it;
			
			for (it = names.begin(); it != names.end(); ++it) {
				if (it->first == beginning) {
					break;
				}
			}

			if (it != names.end()) {
				name = it->second;

				return;
			}

			for (auto type : types) {
				if ((type.size() > package.size()) && (type.substr(0, package.size()) == package) && (type.substr(package.size() + 1) == name)) {
					name = type;
				}
			}
		}
		
		Context clone(const std::string &appender = "") {
			Context clone(types);
			
			clone.package = package;
			
			if (appender == "") {
				clone.path = path;
			} else {
				clone.path = path + "." + appender;
			}
			
			clone.names = names;
			
			return clone;
		}
	};
} }
