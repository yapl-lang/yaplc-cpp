#include "fs.h"
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <stdlib.h>

namespace fs {
	char path::PathDelim = '/';

	path::path() {

	}

	path::path(const std::string &path) {
		if (path.size() == 0) {
			throw "Path cannot be empty.";
		}

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
	}

	path::~path() {

	}

	bool path::create() {
		auto handle = fopen(full_name().c_str(), "wb");

		if (handle != NULL) {
			fclose(handle);

			return true;
		}

		return false;
	}

	bool path::mkdir() {
		return ::mkdir(full_name().c_str(), 0755) == 0;
	}

	bool path::mkdirs() {
		if (exists()) {
			return true;
		}

		return parent().mkdirs() && mkdir();
	}

	bool path::exists() const {
		struct stat st;
		return stat(full_name().c_str(), &st) == 0;
	}

	bool path::is_file() const {
		struct stat st;

		if (stat(full_name().c_str(), &st) != 0) {
			return false;
		}

		return S_ISREG(st.st_mode);
	}

	bool path::is_directory() const {
		struct stat st;

		if (stat(full_name().c_str(), &st) != 0) {
			return false;
		}

		return S_ISDIR(st.st_mode);
	}

	path path::parent() const {
		return path{path()};
	}

	void path::parent(const path &target) {
		if (is_directory()) {
			if (target.is_directory()) {
				rename(full_name().c_str(), (target.full_name() + PathDelim + name()).c_str());
			} else if (target.is_file()) {
				rename(full_name().c_str(), target.full_name().c_str());
			}
		} else if (is_file()) {
			if (target.is_directory()) {
				rename(full_name().c_str(), (target.full_name() + PathDelim + name().c_str()).c_str());
			} else if (target.is_file()) {
				rename(full_name().c_str(), target.full_name().c_str());
			}
		}
	}

	// Folder functions
	path path::add(const std::string &name) {
		path{name}.parent(*this);
	}

	std::vector<path> path::list() const {
		auto handle = opendir(full_name().c_str());

		if (handle == NULL) {
			return {};
		}

		std::vector<path> files;

		while (auto child = readdir(handle)) {
			if ((strcmp(child->d_name, ".") == 0) || (strcmp(child->d_name, "..")) == 0) {
				continue;
			}

			files.push_back((*this)/child->d_name);
		}

		closedir(handle);

		return files;
	}

	// File functions
	std::string path::content() const {
		auto handle = fopen(full_name().c_str(), "rb");

		if (handle == NULL) {
			return {};
		}

		fseek(handle, 0, SEEK_END);
		auto size = (unsigned long)ftell(handle);
		fseek(handle, 0, SEEK_SET);

		std::string buffer;
		buffer.resize(size);

		fread(&buffer[0], sizeof(buffer[0]), size, handle);
		fclose(handle);

		return buffer;
	}

	bool path::content(const std::string &newContent) {
		auto handle = fopen(full_name().c_str(), "wb");

		if (handle != NULL) {
			fwrite(newContent.data(), sizeof(newContent[0]), newContent.size(), handle);
			fclose(handle);

			return true;
		}

		return false;
	}

	// Operators
	path path::operator /(const std::string &sub) const {
		if (!is_directory()) {
			throw "Cannot get path on not a directory.";
		}

		return path{itemPath + PathDelim + itemName + PathDelim + sub};
	}

	path temp() {
		std::string templ;

		{
			auto temp = getenv("TMPDIR");

			if (temp == NULL) {
				templ = "/tmp";
			} else {
				templ = temp;
			}
		}

		templ += "/yaplc.";


		path temp;
		auto t = (unsigned long)time(NULL);

		do {
			char buf[64];
			int c = snprintf(buf, 64, "%lu", t);

			temp = templ + buf;
		} while (temp.exists());

		temp.mkdirs();

		return temp;
	}
}
