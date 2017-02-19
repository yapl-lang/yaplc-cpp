#include "CEmitter.h"
#include <algorithm>

namespace yaplc { namespace cemit {
	static std::string HEADER_H = "/* THIS FILE IS CREATED USING YAPLC */";
	static std::string HEADER_C = "/* THIS FILE IS CREATED USING YAPLC */";

	CEmitter::CEmitter() {

	}

	CEmitter::~CEmitter() {

	}

	void CEmitter::emit(const fs::path &outPath, const structure::RootNode *rootNode) {
		for (auto node : *rootNode) {
			if (auto packageNode = dynamic_cast<structure::PackageNode *>(node)) {
				emit(outPath, packageNode);
			}
		}
	}

	void CEmitter::emit(const fs::path &outPath, const structure::PackageNode *packageNode) {
		for (auto node : *packageNode) {
			if (auto typeNode = dynamic_cast<structure::TypeNode *>(node)) {
				emit(outPath, typeNode);
			}
		}
	}

	void CEmitter::emit(const fs::path &outPath, const structure::TypeNode *typeNode) {
		auto packagePath = ((structure::PackageNode *)typeNode->getListableParent())->name;
		std::replace(packagePath.begin(), packagePath.end(), '.', fs::path::PathDelim);
		auto packageFolder = outPath/packagePath;
		packageFolder.mkdirs();

		auto typeFile = packageFolder.full_name() + fs::path::PathDelim + typeNode->name->type;
		outh.open(typeFile + ".h");
		outc.open(typeFile + ".c");

		outh << HEADER_H << std::endl;
		outc << HEADER_C << std::endl;

		auto moduleHash = ((structure::PackageNode *)typeNode->getListableParent())->name + "." + typeNode->name->type;
		std::replace(moduleHash.begin(), moduleHash.end(), '.', '$');
		std::transform(moduleHash.begin(), moduleHash.end(), moduleHash.begin(), ::toupper);

		outh << std::endl
			<< "#ifndef YAPLC_MODULE_" << moduleHash << std::endl
			<< "#define YAPLC_MODULE_" << moduleHash << std::endl;

		if (auto classNode = dynamic_cast<structure::ClassNode *>(typeNode)) {
			emit(classNode);
		}

		outh << std::endl
			<< "#endif";

		outh.close();
		outc.close();
	}

	void CEmitter::emit(const structure::ClassNode *classNode) {

	}
} }
