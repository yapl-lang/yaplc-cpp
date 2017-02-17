#pragma once

#include <string>
#include <vector>

namespace fs {
	class path {
	public:
		static char PathDelim;

	private:
		std::string itemPath;
		std::string itemName;

	public:
		path();
		path(const std::string &path);
		~path();

		// Common functions
		inline std::string dir() const {
			return itemPath;
		}
		inline std::string name() const {
			return itemName;
		}
		inline std::string full_name() const {
			return dir() + PathDelim + name();
		}

		bool create();
		bool mkdir();
		bool mkdirs();

		bool exists() const;
		bool is_file() const;
		bool is_directory() const;

		path parent() const;
		void parent(const path &target);

		// Folder functions
		path add(const std::string &name);

		std::vector<path> list() const;

		// File functions
		std::string content() const;
		bool content(const std::string &newContent);

		// Operators
		path operator /(const std::string &sub) const;
	};
}
