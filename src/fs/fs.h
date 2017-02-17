#pragma once

#include <string>
#include <vector>

namespace fs {
	class Item {
	public:
		static char PathDelim;

	private:
		std::string itemPath;
		std::string itemName;

	public:
		Item();
		Item(const std::string &path);
		~Item();

		// Common functions
		inline std::string path() const {
			return itemPath;
		}
		inline std::string name() const {
			return itemName;
		}
		inline std::string full_name() const {
			return path() + PathDelim + name();
		}

		bool create();
		bool mkdir();
		bool mkdirs();

		bool exists() const;
		bool is_file() const;
		bool is_directory() const;

		Item parent() const;
		void parent(const Item &target);

		// Folder functions
		Item add(const std::string &name);

		std::vector<Item> list();

		// File functions
		std::string content();
		bool content(const std::string &newContent);

		// Operators
		Item operator /(const std::string &path);
	};

	Item path(const std::string &path);
}
