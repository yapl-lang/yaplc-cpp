#include "Item.h"
#include <dirent.h>
#include <cstring>

namespace fs {
	char Item::PathDelim = '/';

	Item::Item() {

	}

	Item::Item(const std::string &path) {
		if (path.size() == 0) {
			throw "Path cannot be empty.";
		}

		//this->itemPath = ;
		//this->itemName = ;

		auto newPath = path;

		char end = newPath[newPath.size() - 1];
		if ((end == '/') || (end == '\\')) {
			newPath = newPath.substr(0, newPath.size() - 1);
		}

		auto delim = newPath.find_last_of("/\\");
		if (delim == std::string::npos) {
			itemPath = "";
			itemName = newPath;
		} else {
			itemPath = newPath.substr(0, delim);
			itemName = newPath.substr(delim + 1);
		}

		auto handle = opendir(full_name().c_str());

		if (handle == NULL) {
			auto handle2 = fopen(full_name().c_str(), "r");

			if (handle2 == NULL) {
			} else {
				type = Type::File;
				fclose(handle2);
			}
		} else {
			type = Type::Directory;
			closedir(handle);
		}
	}

	Item::~Item() {

	}

	bool Item::exists() const {
		switch (type) {
		case Type::Directory: {
			auto handle = opendir(full_name().c_str());

			if (handle == NULL) {
				return false;
			}

			closedir(handle);
			return true;
		}
		case Type::File: {
			auto handle = fopen(full_name().c_str(), "r");

			if (handle == NULL) {
				return false;
			}

			fclose(handle);
			return true;
		}
		}

		return false;
	}

	Item Item::parent() const {
		return Item{path()};
	}

	void Item::parent(const Item &target) {
		switch (type) {
		case Type::Directory:
			switch (target.type) {
			case Type::Directory:
				rename(full_name().c_str(), (target.full_name() + PathDelim + name()).c_str());
				break;
			case Type::File:
				rename(full_name().c_str(), target.full_name().c_str());
				break;
			}
			break;
		case Type::File:
			switch (target.type) {
			case Type::Directory:
				rename(full_name().c_str(), (target.full_name() + PathDelim + name().c_str()).c_str());
				break;
			case Type::File:
				rename(full_name().c_str(), target.full_name().c_str());
				break;
			}
			break;
		}
	}

	// Folder functions
	Item Item::add(const std::string &name) {
		Item{name}.parent(*this);
	}

	std::vector<Item> Item::list() {
		auto handle = opendir(full_name().c_str());

		if (handle == NULL) {
			return {};
		}

		while (auto child = readdir(handle)) {
			if ((strcmp(child->d_name, ".") == 0) || (strcmp(child->d_name, "..")) == 0) {
				continue;
			}

			printf("%s\n", child->d_name);
		}

		closedir(handle);

		return {};
	}

	// File functions
	std::string Item::content() {

	}

	void Item::content(const std::string &newContent) {

	}

	// Operators
	Item Item::operator /(const std::string &path) {
		if (!is_directory()) {
			throw "Cannot get path on not a directory.";
		}

		return Item{itemPath + PathDelim + itemName + PathDelim + path};
	}


	Item path(const std::string &path) {
		return Item{path};
	}
}
