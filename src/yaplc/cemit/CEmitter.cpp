#include "CEmitter.h"
#include "yaplc/structure/VariableMemberNode.h"
#include "yaplc/util/replace.h"
#include "machineid/machineid.h"
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
		binPath(emitPath/"bin"),
		context(nullptr) {
		emitPath.mkdir();
		includePath.mkdir();
		sourcePath.mkdir();
		objectPath.mkdir();
		binPath.mkdir();

		gcc.argument("-DNULL=0");
		gcc.includePath(includePath);

		contextStack.push_back({});
		context = &contextStack[0];

		generateMain();
	}

	CEmitter::~CEmitter() {

	}

	void CEmitter::generateMain() {
		static std::vector<std::string> objects {
			"yapl/class",
			"yapl/main",
			"yapl/memory",
			"yapl/objectref",
			"yapl/yapl"
		};

		auto emitData = fs::relative("./data/c");
		auto includeDirectory = emitData/"include";
		auto srcDirectory = emitData/"src";

		if (!emitData.exists()) {
			throw std::runtime_error("The path \"" + emitData.full_name() + "\" is not exist.");
		}

		if (!includeDirectory.exists()) {
			throw std::runtime_error("The path \"" + includeDirectory.full_name() + "\" is not exist.");
		}

		if (!srcDirectory.exists()) {
			throw std::runtime_error("The path \"" + srcDirectory.full_name() + "\" is not exist.");
		}

		gcc.includePath(includeDirectory);

		auto objDirectory = emitData/"obj";

		auto machineHash = machineid::machineHash();
		auto machineHashFile = emitData/".machine";

		if ((!machineHashFile.exists()) || (machineHashFile.content() != machineHash)) {
			machineHashFile.content(machineHash);

			objDirectory.remove();
		}

		objDirectory.mkdir();

		for (auto object : objects) {
			auto srcFile = srcDirectory/(object + ".c");
			auto objFile = objDirectory/(object + ".o");

			gcc.compile(srcFile, objFile);
		}
	}

	void CEmitter::push() {
		contextStack.push_back(*context);
		context = &*(--contextStack.end());
	}

	void CEmitter::pop() {
		contextStack.pop_back();
		context = &*(--contextStack.end());
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

		auto moduleHash = typeNode->name->type;
		std::replace(moduleHash.begin(), moduleHash.end(), '.', '$');
		std::replace(moduleHash.begin(), moduleHash.end(), '<', '$');
		std::replace(moduleHash.begin(), moduleHash.end(), '>', '$');
		std::transform(moduleHash.begin(), moduleHash.end(), moduleHash.begin(), ::toupper);

		if (auto classNode = dynamic_cast<const structure::ClassNode *>(typeNode)) {
			emit(classNode);
		}

		outstreamh.open(packageHeader.full_name());
		outstreamc.open(packageSource.full_name());
		outh.header(HEADER_H).includeGuard("YAPL_MODULE_" + moduleHash).write(outstreamh).reset();
		outc.header(HEADER_C).include(includePath.relative(packageHeader)).write(outstreamc).reset();
		gcc.compile(packageSource, packageObject);
		outstreamh.close();
		outstreamc.close();

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
		outh.include("yapl/class.h");

		auto classSymbolName = convertName(classNode->name->type);

		outh << CodeStream::NewLine
			<< "struct " << classSymbolName << "$class " << CodeStream::OpenBlock << CodeStream::NewLine
			<< "struct yapl$class $common;" << CodeStream::NewLine;
		CodeStream::CodeBackup initializatorBackup;
		{
			CodeStream initializator;
			placeVTable(classNode, outh, initializator);
			initializator.write(initializatorBackup);
		}
		outh << CodeStream::CloseBlock << ";" << CodeStream::NewLine;

		outh << CodeStream::NewLine
			<< "struct " << classSymbolName << " " << CodeStream::OpenBlock << CodeStream::NewLine
			<< "struct " << classSymbolName << "$class *$class;" << CodeStream::NewLine;
		for (auto node : *classNode) {
			if (auto memberNode = dynamic_cast<structure::MemberNode *>(node)) {
				auto child = memberNode->get();

				if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {

				} else if (auto variableMemberNode = dynamic_cast<structure::VariableMemberNode *>(child)) {
					if (memberNode->staticality == structure::MemberNode::Staticality::Dynamic) {
						outh << requestTypeRef(memberNode->type) << " " << getLast(memberNode->getName()) << ";" << CodeStream::NewLine;
					}
				}
			} else if (auto specialNode = dynamic_cast<structure::SpecialNode *>(node)) {
				emitInStruct(specialNode);
			}
		}
		outh << CodeStream::CloseBlock << ";" << CodeStream::NewLine;

		outc << CodeStream::NewLine
			<< "struct " << classSymbolName << "$class " << classSymbolName << "$class() " << CodeStream::OpenBlock << CodeStream::NewLine
				<< "static struct " << classSymbolName << "$class *instance = NULL;" << CodeStream::NewLine
				<< CodeStream::NewLine
				<< "if (instance == NULL) " << CodeStream::OpenBlock << CodeStream::NewLine
					<< "instance = malloc(struct " << classSymbolName << "$class);" << CodeStream::NewLine
					<< CodeStream::NewLine
				<< CodeStream::CloseBlock << CodeStream::NewLine
				<< CodeStream::NewLine
				<< initializatorBackup << CodeStream::NewLine
				<< "return instance;" << CodeStream::NewLine
				<< CodeStream::CloseBlock << CodeStream::NewLine
			<< CodeStream::NewLine;

		outh << CodeStream::NewLine
			<< "struct yapl$objectref " << classSymbolName << "$create();" << CodeStream::NewLine
			<< CodeStream::NewLine;

		outc << CodeStream::NewLine
			<< "struct yapl$objectref " << classSymbolName << "$create() " << CodeStream::OpenBlock << CodeStream::NewLine
			<< "struct yapl$objectref result;" << CodeStream::NewLine
			<< "yapl$objectref$init(result, malloc(sizeof(struct " << classSymbolName << ")));" << CodeStream::NewLine
			<< "return result;" << CodeStream::NewLine
			<< CodeStream::CloseBlock << CodeStream::NewLine
			<< CodeStream::NewLine;

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

	void CEmitter::emit(const structure::VariableMemberNode *variableMemberNode) {

	}

	void CEmitter::emit(const structure::MethodMemberNode *methodMemberNode) {
		if (auto memberNode = dynamic_cast<structure::MemberNode *>(methodMemberNode->getContainerParent())) {
			outh << requestTypeRef(memberNode->type) << " " << getFullMethodName(methodMemberNode);
			showArguments(outh, methodMemberNode->arguments, dynamic_cast<const structure::TypeNode *>(memberNode->getParent()));
			outh << ";" << CodeStream::NewLine;

			outc << CodeStream::NewLine;
			outc << requestTypeRef(memberNode->type) << " " << getFullMethodName(methodMemberNode);
			showArguments(outc, methodMemberNode->arguments, dynamic_cast<const structure::TypeNode *>(memberNode->getParent()));
			outc << " " << CodeStream::OpenBlock << CodeStream::NewLine;
			emit(methodMemberNode->body);
			outc << CodeStream::CloseBlock << CodeStream::NewLine;
		}
	}

	void CEmitter::emit(const structure::ExpressionNode *expressionNode) {
		push();
		for (auto node : *expressionNode) {
			if (auto specialNode = dynamic_cast<structure::SpecialNode *>(node)) {
				emitInStruct(specialNode);
			}
		}
		pop();
	}

	void CEmitter::placeVTable(const structure::ClassNode *classNode, CodeStream &structMembers, CodeStream &initializator) {
		if (auto parentClass = dynamic_cast<structure::ClassNode *>(getType(classNode->base->type))) {
			if (classNode == parentClass) {
				if (classNode->name->type == "yapl.Object") {
					structMembers << "void (*yapl$handleGC)();" << CodeStream::NewLine;
				} else {
					// TODO: Error, recursive extend
				}
			} else {
				placeVTable(parentClass, structMembers, initializator);
			}
		}

		for (auto node : *classNode) {
			if (auto memberNode = dynamic_cast<structure::MemberNode *>(node)) {
				auto child = memberNode->get();

				if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {
					auto shortMethodName = getShortMethodName(methodMemberNode);

					structMembers << requestTypeRef(memberNode->type) << " (*" << shortMethodName << ")";
					showArguments(structMembers, methodMemberNode->arguments, classNode);
					structMembers << ";" << CodeStream::NewLine;

					initializator << "instance->" << shortMethodName << " = " << getFullMethodName(methodMemberNode) << ";" << CodeStream::NewLine;
				}
			}
		}
	}

	void CEmitter::showArguments(CodeStream &stream, const structure::ArgumentsNode *argumentsNode, const structure::TypeNode *typeNode) {
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
			outh << "char *buffer;" << CodeStream::NewLine;
			outh << "unsigned long length;" << CodeStream::NewLine;
		} else if (data == "yapl.Array") {
			// Template argument
			structure::TypeNameNode typeNameNode;
			typeNameNode.type = "T";
			outh << "" << requestTypeRef(&typeNameNode) << " *elements;" << CodeStream::NewLine;
			outh << "unsigned long count;" << CodeStream::NewLine;
		} else if (data == "yapl.Type") {

		} else {
			printf("%s\n", data.c_str());
			// TODO: error
		}
	}

	void CEmitter::emitInMethod(const structure::SpecialNode *specialNode) {
		auto data = specialNode->data;

		if (data == "yapl.String.constructor") {
			outh << "((yapl$String *)$this->target)->buffer = calloc(0, 1);" << CodeStream::NewLine;
		} else if (data == "yapl.String.constructor.string") {
			outh << CodeStream::NewLine;
		} else {
			printf("%s\n", data.c_str());
			// TODO: error
		}
	}

	void CEmitter::build() {
		gcc.build(fs::escape(binPath/"exe"));
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
					result += "$this";
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
			return convertName(getNotLast(memberNode->getName())) + "$" + getShortMethodName(methodMemberNode);
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
