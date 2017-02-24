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


		generateMain();

		{
			auto rootNode = new structure::RootNode();

			auto packageNode = new structure::PackageNode();
			packageNode->name = "yapl";
			rootNode->add(packageNode);

			auto classNode = new structure::ClassNode();
			packageNode->add(classNode);

			classNode->name = new structure::TypeNameNode();
			classNode->name->type = "Object";

			auto memberNode = new structure::MemberNode();
			memberNode->setName("getType");
			classNode->add(memberNode);
			memberNode->type = new structure::TypeNameNode();
			memberNode->type->type = "yapl.Type";

			auto methodMemberNode = new structure::MethodMemberNode();
			memberNode->set(methodMemberNode);

			auto specialNode = new structure::SpecialNode();
			methodMemberNode->body->add(specialNode);
			specialNode->data = "yapl$class$getType";

			addObject(rootNode);
			emit(packageNode);
		}
	}

	CEmitter::~CEmitter() {

	}

	void CEmitter::addObject(structure::RootNode *rootNode) {

	}

	void CEmitter::generateMain() {
		{
			auto packageHeader = includePath/"yapl/main.h";
			auto packageSource = sourcePath/"yapl/main.c";
			auto packageObject = objectPath/"yapl/main.o";
			packageHeader.parent().mkdirs();
			packageSource.parent().mkdirs();
			packageObject.parent().mkdirs();
			outh.open(packageHeader.full_name());
			outc.open(packageSource.full_name());

			outh << HEADER_H << std::endl;
			outc << HEADER_C << std::endl;

			outh << std::endl
				<< "#ifndef YAPL_MAIN" << std::endl
				<< "#define YAPL_MAIN" << std::endl
				<< "#include \"yapl/yapl.h\"" << std::endl;
			outc << std::endl
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << std::endl;

			outc << std::endl
				<< "int main(char **argc, int argv) {" << std::endl
				<< "\treturn yapl$main(argc, argv);" << std::endl
				<< "}" << std::endl;

			outh << std::endl
				<< "#endif";

			files.push_back({
				packageHeader,
				packageSource,
				packageObject
			});
			outh.close();
			outc.close();
		}{
			auto packageHeader = includePath/"yapl/yapl.h";
			auto packageSource = sourcePath/"yapl/yapl.c";
			auto packageObject = objectPath/"yapl/yapl.o";
			packageHeader.parent().mkdirs();
			packageSource.parent().mkdirs();
			packageObject.parent().mkdirs();
			outh.open(packageHeader.full_name());
			outc.open(packageSource.full_name());

			outh << HEADER_H << std::endl;
			outc << HEADER_C << std::endl;

			outh << std::endl
				<< "#ifndef YAPL_YAPL" << std::endl
				<< "#define YAPL_YAPL" << std::endl
				<< "#include \"yapl/class.h\"" << std::endl;
			outc << std::endl
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << std::endl;

			outh << std::endl
				<< "int yapl$main(char **argc, int argv);" << std::endl;
			outc << std::endl
				<< "int yapl$main(char **argc, int argv) {" << std::endl
				<< "}" << std::endl;

			outh << std::endl
				<< "#endif";

			files.push_back({
				packageHeader,
				packageSource,
				packageObject
			});
			outh.close();
			outc.close();
		}{
			auto packageHeader = includePath/"yapl/class.h";
			auto packageSource = sourcePath/"yapl/class.c";
			auto packageObject = objectPath/"yapl/class.o";
			packageHeader.parent().mkdirs();
			packageSource.parent().mkdirs();
			packageObject.parent().mkdirs();
			outh.open(packageHeader.full_name());
			outc.open(packageSource.full_name());

			outh << HEADER_H << std::endl;
			outc << HEADER_C << std::endl;

			outh << std::endl
				<< "#ifndef YAPL_CLASS" << std::endl
				<< "#define YAPL_CLASS" << std::endl;
			outc << std::endl
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << std::endl;

			outh << std::endl
				<< "struct yapl$class {" << std::endl
				<< "\tstruct yapl$class *parent;" << std::endl
				<< "\tstruct yapl$class **interfaces;" << std::endl
				<< "\tchar *name;" << std::endl
				<< "\tvoid **vtable;" << std::endl
				<< "};" << std::endl;

			outh << std::endl
				<< "#endif";

			files.push_back({
				packageHeader,
				packageSource,
				packageObject
			});
			outh.close();
			outc.close();
		}
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
			<< "#ifndef YAPL_MODULE_" << moduleHash << std::endl
			<< "#define YAPL_MODULE_" << moduleHash << std::endl
			<< "#include \"yapl/yapl.h\"" << std::endl;
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
		outh << std::endl
			<< "struct " << classNode->name->type << " {" << std::endl
			<< "\tstruct yapl$class *$class;" << std::endl
			<< "};" << std::endl;
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
