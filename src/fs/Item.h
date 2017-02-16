#pragma once

#include <string>
#include <vector>

namespace fs {
	class Item {
	private:
		enum class Type {File, Directory};

	public:
		static char PathDelim;

	private:
		Type type;

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

		bool exists() const;
		inline bool is_file() const {
			return type == Type::File;
		}
		inline bool is_directory() const {
			return type == Type::Directory;
		}

		Item parent() const;
		void parent(const Item &target);

		// Folder functions
		Item add(const std::string &name);

		std::vector<Item> list();

		// File functions
		std::string content();
		void content(const std::string &newContent);

		// Operators
		Item operator /(const std::string &path);
	};

	Item path(const std::string &path);
}
