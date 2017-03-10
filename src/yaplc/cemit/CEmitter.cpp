#include "CEmitter.h"
#include "yaplc/structure/VariableMemberNode.h"
#include "yaplc/util/replace.h"
#include <algorithm>
#include <stdlib.h>
#include <iostream>

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
	}

	CEmitter::~CEmitter() {

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
				<< "#include <stdlib.h>" << std::endl
				<< std::endl
				<< "#include \"yapl/class.h\"" << std::endl
				<< "#include \"yapl/objectref.h\"" << std::endl;
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
		}{
			auto packageHeader = includePath/"yapl/objectref.h";
			auto packageSource = sourcePath/"yapl/objectref.c";
			auto packageObject = objectPath/"yapl/objectref.o";
			packageHeader.parent().mkdirs();
			packageSource.parent().mkdirs();
			packageObject.parent().mkdirs();
			outh.open(packageHeader.full_name());
			outc.open(packageSource.full_name());

			outh << HEADER_H << std::endl;
			outc << HEADER_C << std::endl;

			outh << std::endl
				<< "#ifndef YAPL_OBJECTREF" << std::endl
				<< "#define YAPL_OBJECTREF" << std::endl
				<< "#include \"Object.h\"" << std::endl;
			outc << std::endl
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << std::endl;

			outh << std::endl
				<< "struct yapl$objectref$item {" << std::endl
				<< "\tstruct Object *target;" << std::endl
				<< "\tunsigned long count;" << std::endl
				<< "};" << std::endl;

			outh << std::endl
				<< "struct yapl$objectref {" << std::endl
				<< "\tstruct yapl$objectref$item *item;" << std::endl
				<< "};" << std::endl;

			outh << std::endl
				<< "void yapl$objectref$init(struct yapl$objectref ref, struct Object *object);" << std::endl
				<< "void yapl$objectref$push(struct yapl$objectref ref);" << std::endl
				<< "void yapl$objectref$pop(struct yapl$objectref ref);" << std::endl;

			outc << std::endl
				<< "void yapl$objectref$init(struct yapl$objectref ref, struct Object *object) {" << std::endl
				<< "\tref.item = malloc(sizeof(struct yapl$objectref$item));" << std::endl
				<< "\tref.item->count = 1;" << std::endl
				<< "\tref.item->target = object;" << std::endl
				<< "}" << std::endl
				<< std::endl
				<< "void yapl$objectref$push(struct yapl$objectref ref) {" << std::endl
				<< "\t++ref.item->count;" << std::endl
				<< "}" << std::endl
				<< std::endl
				<< "void yapl$objectref$pop(struct yapl$objectref ref) {" << std::endl
				<< "\tif (ref.item->count == 1) {" << std::endl
				<< "\t\tfree(ref.item->target);" << std::endl
				<< "\t\tfree(ref.item);" << std::endl
				<< "\t} else {" << std::endl
				<< "\t\t--ref.item->count;" << std::endl
				<< "\t}" << std::endl
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
		if (typeNode->name->templateArguments.size() != 0) {
			return;
		}

		auto packagePath = getNotLast(typeNode->name->type);
		std::replace(packagePath.begin(), packagePath.end(), '.', fs::path::PathDelim);

		auto name = shortName(typeNode->name);
		auto packageHeader = includePath/packagePath/(name + ".h");
		auto packageSource = sourcePath/packagePath/(name + ".c");
		auto packageObject = objectPath/packagePath/(name + ".o");
		packageHeader.parent().mkdirs();
		packageSource.parent().mkdirs();
		packageObject.parent().mkdirs();
		outh.open(packageHeader.full_name());
		outc.open(packageSource.full_name());

		outh << HEADER_H << std::endl;
		outc << HEADER_C << std::endl;

		auto moduleHash = typeNode->name->type;
		std::replace(moduleHash.begin(), moduleHash.end(), '.', '$');
		std::replace(moduleHash.begin(), moduleHash.end(), '<', '$');
		std::replace(moduleHash.begin(), moduleHash.end(), '>', '$');
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

		for (auto node : *typeNode) {
			if (auto subTypeNode = dynamic_cast<structure::TypeNode *>(node)) {
				emit(subTypeNode);
			}
		}
	}

	void CEmitter::emit(const structure::ClassNode *classNode) {
		auto classSymbolName = convertName(classNode->name->type);

		outh << std::endl
			<< "struct " << classSymbolName << "$class {" << std::endl
			<< "\tstruct yapl$class $common;" << std::endl;
		placeVTable(classNode);
		outh << "};" << std::endl;

		outh << std::endl
			<< "struct " << classSymbolName << " {" << std::endl
			<< "\tstruct " << classSymbolName << "$class *$class;" << std::endl;
		for (auto node : *classNode) {
			if (auto memberNode = dynamic_cast<structure::MemberNode *>(node)) {
				auto child = memberNode->get();

				if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {

				} else if (auto variableMemberNode = dynamic_cast<structure::VariableMemberNode *>(child)) {
					if (memberNode->staticality == structure::MemberNode::Staticality::Dynamic) {
						outh << "\t" << requestTypeRef(memberNode->type) << " " << getLast(memberNode->getName()) << ";" << std::endl;
					}
				}
			} else if (auto specialNode = dynamic_cast<structure::SpecialNode *>(node)) {
				emitInStruct(specialNode);
			}
		}
		outh << "};" << std::endl;

		outh << std::endl
			<< "struct yapl$objectref " << classSymbolName << "$create();" << std::endl
			<< std::endl;

		outc << std::endl
			<< "struct yapl$objectref " << classSymbolName << "$create() {" << std::endl
			<< "\tstruct yapl$objectref result;" << std::endl
			<< "\tyapl$objectref$init(result, malloc(sizeof(struct " << classSymbolName << ")));" << std::endl
			<< "\treturn result;" << std::endl
			<< "}" << std::endl
			<< std::endl;

		for (auto node : *classNode) {
			if (auto memberNode = dynamic_cast<structure::MemberNode *>(node)) {
				auto child = memberNode->get();

				if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {
					emit(methodMemberNode);
				}
			}
		}
	}

	void CEmitter::emit(const structure::MemberNode *memberNode) {
		auto child = memberNode->get();

		if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {
			emit(methodMemberNode);
		} else if (auto variableMemberNode = dynamic_cast<structure::VariableMemberNode *>(child)) {
			emit(variableMemberNode);
		}
	}

	void CEmitter::emit(const structure::MethodMemberNode *methodMemberNode) {
		if (auto memberNode = dynamic_cast<structure::MemberNode *>(methodMemberNode->getContainerParent())) {
			outh << requestTypeRef(memberNode->type) << " " << getFullMethodName(methodMemberNode);
			showArguments(outh, methodMemberNode->arguments, dynamic_cast<const structure::TypeNode *>(memberNode->getParent()));
			outh << ";" << std::endl;

			outc << std::endl;
			outc << requestTypeRef(memberNode->type) << " " << getFullMethodName(methodMemberNode);
			showArguments(outc, methodMemberNode->arguments, dynamic_cast<const structure::TypeNode *>(memberNode->getParent()));
			outc << " {" << std::endl;

			outc << "}" << std::endl;
		}
	}

	void CEmitter::emit(const structure::VariableMemberNode *variableMemberNode) {

	}

	void CEmitter::placeVTable(const structure::ClassNode *classNode) {
		if (auto parentClass = dynamic_cast<structure::ClassNode *>(getType(classNode->base->type))) {
			if (classNode == parentClass) {
				if (classNode->name->type != "yapl.Object") {
					// TODO: Error, recursive extend
				}
			} else {
				placeVTable(parentClass);
			}
		}

		for (auto node : *classNode) {
			if (auto memberNode = dynamic_cast<structure::MemberNode *>(node)) {
				auto child = memberNode->get();

				if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {
					outh << "\t" << requestTypeRef(memberNode->type) << " (*" << getShortMethodName(methodMemberNode) << ")";
					showArguments(outh, methodMemberNode->arguments, classNode);
					outh << ";" << std::endl;
				}
			}
		}
	}

	void CEmitter::showArguments(std::ostream &stream, const structure::ArgumentsNode *argumentsNode, const structure::TypeNode *typeNode) {
		stream << "(";

		bool printComma = false;

		structure::TypeNameNode *type;
		std::string name;
		structure::ExpressionNode *value;

		auto printArgument = [this, &stream, &type, &name, &value]() {
			stream << requestTypeRef(type) << " " << name;
		};

		if (typeNode != nullptr) {
			type = typeNode->name;
			name = "$this";
			printComma = true;
			printArgument();
		}

		auto arguments = argumentsNode->arguments;
		auto it = arguments.begin();

		if (it != arguments.end()) {
			std::tie(type, name, value) = *it;

			if (printComma) {
				stream << ", ";
			}

			printComma = true;
			printArgument();

			++it;

			for (; it != arguments.end(); ++it) {
				stream << ", ";

				std::tie(type, name, value) = *it;
				printArgument();
			}
		}

		stream << ")";
	}

	std::string CEmitter::requestTypeRef(const structure::TypeNameNode *typeNameNode, bool prependStruct) {
		static std::map<std::string, std::string> typeNameMapping {
			{"bool", "unsigned char"},
			{"char", "char"},

			{"byte", "unsigned char"},
			{"sbyte", "signed char"},
			{"short", "signed short"},
			{"ushort", "unsigned short"},
			{"int", "signed long"},
			{"uint", "unsigned long"},
			{"long", "signed long long"},
			{"ulong", "unsigned long long"},

			{"float", "float"},
			{"double", "double"},

			{"void", "void"},

			{"size", "unsigned long"}
		};

		auto typeName = typeNameNode->hashName();

		auto it = typeNameMapping.find(typeName);
		if (it != typeNameMapping.end()) {
			return it->second;
		}

		auto type = getType(typeName);
		if (type == nullptr) {
			// TODO: error undefined type

			return "void";
		}

		if (prependStruct) {
			return "struct yapl$objectref";
		}

		return "yapl$objectref";
	}

	std::string CEmitter::requestType(const structure::TypeNameNode *typeNameNode, bool prependStruct) {
		static std::map<std::string, std::string> typeNameMapping {
			{"bool", "unsigned char"},
			{"char", "char"},

			{"byte", "unsigned char"},
			{"sbyte", "signed char"},
			{"short", "signed short"},
			{"ushort", "unsigned short"},
			{"int", "signed long"},
			{"uint", "unsigned long"},
			{"long", "signed long long"},
			{"ulong", "unsigned long long"},

			{"float", "float"},
			{"double", "double"},

			{"void", "void"},

			{"size", "unsigned long"}
		};

		auto typeName = typeNameNode->hashName();

		auto it = typeNameMapping.find(typeName);
		if (it != typeNameMapping.end()) {
			return it->second;
		}

		auto type = getType(typeName);
		if (type == nullptr) {
			// TODO: error undefined type

			return "void";
		}

		if (prependStruct) {
			return "struct " + convertName(type->name->type);
		}

		return convertName(type->name->type);
	}

	void CEmitter::emitInStruct(const structure::SpecialNode *specialNode) {
		auto data = specialNode->data;

		if (data == "yapl.String") {
			outh << "\tchar *buffer;" << std::endl;
			outh << "\tunsigned long length;" << std::endl;
		} else if (data == "yapl.Array") {
			// Template argument
			structure::TypeNameNode typeNameNode;
			typeNameNode.type = "T";
			outh << "\t" << requestTypeRef(&typeNameNode) << " *elements;" << std::endl;
			outh << "\tunsigned long count;" << std::endl;
		} else if (data == "yapl.Type") {

		} else {
			printf("%s\n", data.c_str());
			// TODO: error
		}
	}

	void CEmitter::emitInMethod(const structure::SpecialNode *specialNode) {
		auto data = specialNode->data;

		if (data == "yapl.String.constructor") {
			outh << "\t((yapl$String *)$this->target)->buffer = calloc(0, 1);" << std::endl;
		} else if (data == "yapl.String.constructor.string") {
			outh << "\t" << std::endl;
		} else {
			printf("%s\n", data.c_str());
			// TODO: error
		}
	}

	void CEmitter::build() {
		for (auto file : files) {
			system(("gcc -I\"" +
				util::replace(fs::escape(includePath), "$", "\\$") +
				"\" -c \"" +
				util::replace(fs::escape(file.source), "$", "\\$") +
				"\" -o \"" +
					util::replace(fs::escape(file.object), "$", "\\$") +
				"\"").c_str());
		}

		std::stringstream ss;
		ss << "gcc ";
		for (auto file : files) {
			ss << "\"" << util::replace(fs::escape(file.object), "$", "\\$") << "\" ";
		}
		ss << "-o \"" << util::replace(fs::escape(binPath/"exe"), "$", "\\$") << "\"";

		system(ss.str().c_str());
	}

	std::string CEmitter::convertName(const std::string &original) {
		auto copy = original;
		std::replace(copy.begin(), copy.end(), '.', '$');
		std::replace(copy.begin(), copy.end(), '<', '$');
		std::replace(copy.begin(), copy.end(), '>', '$');
		std::replace(copy.begin(), copy.end(), '[', '$');
		std::replace(copy.begin(), copy.end(), ']', '$');
		return copy;
	}

	std::string CEmitter::getNotLast(const std::string &name) {
		auto pos = name.find_last_of('.');

		if (pos == std::string::npos) {
			return name;
		}

		return name.substr(0, pos);
	}

	std::string CEmitter::getLast(const std::string &name) {
		auto pos = name.find_last_of('.');

		if (pos == std::string::npos) {
			return name;
		}

		return name.substr(pos + 1);
	}

	std::string CEmitter::getShortMethodName(const structure::MethodMemberNode *methodMemberNode) {
		if (auto memberNode = dynamic_cast<structure::MemberNode *>(methodMemberNode->getParent())) {
			auto result = getLast(memberNode->getName());

			switch (memberNode->staticality) {
			case structure::MemberNode::Staticality::Dynamic:
				if (auto typeNode = dynamic_cast<structure::TypeNode *>(memberNode->getParent())) {
					auto selfType = requestType(typeNode->name, false);
					selfType.erase(std::remove_if(selfType.begin(), selfType.end(), ::isspace), selfType.end());
					result += "$" + selfType;
				}
				break;
			case structure::MemberNode::Staticality::Static:
				break;
			}

			for (auto argument : methodMemberNode->arguments->arguments) {
				structure::TypeNameNode *type;
				std::string name;
				structure::ExpressionNode *value;

				std::tie(type, name, value) = argument;

				auto argumentType = requestType(type, false);
				argumentType.erase(std::remove_if(argumentType.begin(), argumentType.end(), ::isspace), argumentType.end());
				result += "$" + argumentType;
			}

			return result;
		}

		return "";
	}

	std::string CEmitter::getFullMethodName(const structure::MethodMemberNode *methodMemberNode) {
		if (auto memberNode = dynamic_cast<structure::MemberNode *>(methodMemberNode->getParent())) {
			return convertName(getNotLast(memberNode->getName())) + getShortMethodName(methodMemberNode);
		}

		return "";
	}

	std::string CEmitter::shortName(const structure::TypeNameNode *typeNameNode) const {
		auto name = typeNameNode->shortType();
		std::replace(name.begin(), name.end(), '.', '$');
		std::replace(name.begin(), name.end(), '<', '$');
		std::replace(name.begin(), name.end(), '>', '$');
		std::replace(name.begin(), name.end(), '[', '$');
		std::replace(name.begin(), name.end(), ']', '$');

		return name;
	}
} }
