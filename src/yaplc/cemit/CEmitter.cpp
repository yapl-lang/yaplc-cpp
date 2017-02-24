#include "CEmitter.h"
#include <algorithm>
#include <stdlib.h>

namespace yaplc { namespace cemit {
	static std::string HEADER_H = "/* THIS FILE IS CREATED USING YAPLC */";
	static std::string HEADER_C = "/* THIS FILE IS CREATED USING YAPLC */";

	CEmitter::CEmitter(const fs::path &outPath) :
		emitPath(outPath/"c"),
		includePath(emitPath/"include"),
		sourcePath(emitPath/"src"),
		objectPath(emitPath/"obj"),
		binPath(emitPath/"bin") {
		emitPath.mkdir();
		includePath.mkdir();
		sourcePath.mkdir();
		objectPath.mkdir();
		binPath.mkdir();
	}

	CEmitter::~CEmitter() {

	}

	void CEmitter::emit(const structure::RootNode *rootNode) {
		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				emit(packageNode);
			}
		}
	}

	void CEmitter::emit(const structure::PackageNode *packageNode) {
		for (auto node : *packageNode) {
			if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
				emit(typeNode);
			}
		}
	}

	void CEmitter::emit(const structure::TypeNode *typeNode) {
		auto packagePath = ((structure::PackageNode *)typeNode->getListableParent())->name;
		std::replace(packagePath.begin(), packagePath.end(), '.', fs::path::PathDelim);

		auto packageHeader = includePath/packagePath/(typeNode->name->type + ".h");
		auto packageSource = sourcePath/packagePath/(typeNode->name->type + ".c");
		auto packageObject = objectPath/packagePath/(typeNode->name->type + ".o");
		packageHeader.parent().mkdirs();
		packageSource.parent().mkdirs();
		packageObject.parent().mkdirs();
		outh.open(packageHeader.full_name());
		outc.open(packageSource.full_name());

		outh << HEADER_H << std::endl;
		outc << HEADER_C << std::endl;

		auto moduleHash = ((structure::PackageNode *)typeNode->getListableParent())->name + "." + typeNode->name->type;
		std::replace(moduleHash.begin(), moduleHash.end(), '.', '$');
		std::transform(moduleHash.begin(), moduleHash.end(), moduleHash.begin(), ::toupper);

		outh << std::endl
			<< "#ifndef YAPLC_MODULE_" << moduleHash << std::endl
			<< "#define YAPLC_MODULE_" << moduleHash << std::endl;
		outc << std::endl
			<< "#include \"" << includePath.relative(packageHeader) << "\"" << std::endl;

		if (auto classNode = dynamic_cast<const structure::ClassNode *>(typeNode)) {
			emit(classNode);
		}

		outh << std::endl
			<< "#endif";

		outh.close();
		outc.close();

		files.push_back({
			packageHeader,
			packageSource,
			packageObject
		});
	}

	void CEmitter::emit(const structure::ClassNode *classNode) {

	}

	void CEmitter::build() {
		for (auto file : files) {
			system(("gcc -I\"" + fs::escape(includePath) + "\" -c \"" + fs::escape(file.source) + "\" -o \"" + fs::escape(file.object) + "\"").c_str());
		}

		std::stringstream ss;
		ss << "gcc ";
		for (auto file : files) {
			ss << "\"" << fs::escape(file.object) << "\" ";
		}
		ss << "-o \"" << fs::escape(binPath/"exe") << "\"";

		system(ss.str().c_str());
	}
} }
