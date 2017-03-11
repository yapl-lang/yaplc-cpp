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
		binPath(emitPath/"bin"),
		context(nullptr) {
		emitPath.mkdir();
		includePath.mkdir();
		sourcePath.mkdir();
		objectPath.mkdir();
		binPath.mkdir();

		contextStack.push_back({});
		context = &contextStack[0];

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

			outh << HEADER_H << endl(this);
			outc << HEADER_C << endl(this);

			outh << endl(this)
				<< "#ifndef YAPL_MAIN" << endl(this)
				<< "#define YAPL_MAIN" << endl(this)
				<< "#include \"yapl/yapl.h\"" << endl(this);
			outc << endl(this)
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << endl(this);

			outc << endl(this)
				<< "int main(char **argc, int argv) {" << pushindent(this) << endl(this)
				<< "return yapl$main(argc, argv);" << endl(this)
				<< pop(this) << "}" << endl(this);

			outh << endl(this)
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

			outh << HEADER_H << endl(this);
			outc << HEADER_C << endl(this);

			outh << endl(this)
				<< "#ifndef YAPL_YAPL" << endl(this)
				<< "#define YAPL_YAPL" << endl(this)
				<< "#include <stdlib.h>" << endl(this)
				<< endl(this)
				<< "#include \"yapl/class.h\"" << endl(this)
				<< "#include \"yapl/objectref.h\"" << endl(this);
			outc << endl(this)
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << endl(this);

			outh << endl(this)
				<< "int yapl$main(char **argc, int argv);" << endl(this);
			outc << endl(this)
				<< "int yapl$main(char **argc, int argv) {" << endl(this)
				<< "}" << endl(this);

			outh << endl(this)
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

			outh << HEADER_H << endl(this);
			outc << HEADER_C << endl(this);

			outh << endl(this)
				<< "#ifndef YAPL_CLASS" << endl(this)
				<< "#define YAPL_CLASS" << endl(this);
			outc << endl(this)
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << endl(this);

			outh << endl(this)
				<< "struct yapl$class {" << pushindent(this) << endl(this)
				<< "struct yapl$class *parent;" << endl(this)
				<< "struct yapl$class **interfaces;" << endl(this)
				<< "char *name;" << endl(this)
				<< "void **vtable;" << endl(this)
				<< pop(this) << "};" << endl(this);

			outh << endl(this)
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

			outh << HEADER_H << endl(this);
			outc << HEADER_C << endl(this);

			outh << endl(this)
				<< "#ifndef YAPL_OBJECTREF" << endl(this)
				<< "#define YAPL_OBJECTREF" << endl(this)
				<< "#include \"Object.h\"" << endl(this);
			outc << endl(this)
				<< "#include \"" << includePath.relative(packageHeader) << "\"" << endl(this);

			outh << endl(this)
				<< "struct yapl$objectref$item {"<< pushindent(this)  << endl(this)
				<< "struct Object *target;" << endl(this)
				<< "unsigned long count;" << endl(this)
				<< pop(this) << "};" << endl(this);

			outh << endl(this)
				<< "struct yapl$objectref {" << pushindent(this) << endl(this)
				<< "struct yapl$objectref$item *item;" << endl(this)
				<< pop(this) << "};" << endl(this);

			outh << endl(this)
				<< "void yapl$objectref$init(struct yapl$objectref ref, struct Object *object);" << endl(this)
				<< "void yapl$objectref$push(struct yapl$objectref ref);" << endl(this)
				<< "void yapl$objectref$pop(struct yapl$objectref ref);" << endl(this);

			outc << endl(this)
				<< "void yapl$objectref$init(struct yapl$objectref ref, struct Object *object) {" << pushindent(this) << endl(this)
				<< "ref.item = malloc(sizeof(struct yapl$objectref$item));" << endl(this)
				<< "ref.item->count = 1;" << endl(this)
				<< "ref.item->target = object;" << endl(this)
				<< pop(this) << "}" << endl(this)
				<< endl(this)
				<< "void yapl$objectref$push(struct yapl$objectref ref) {" << pushindent(this) << endl(this)
				<< "++ref.item->count;" << endl(this)
				<< pop(this) << "}" << endl(this)
				<< endl(this)
				<< "void yapl$objectref$pop(struct yapl$objectref ref) {" << pushindent(this) << endl(this)
				<< "if (ref.item->count == 1) {" << pushindent(this) << endl(this)
				<< "free(ref.item->target);" << endl(this)
				<< "free(ref.item);" << endl(this)
				<< pop(this) << "} else {" << pushindent(this) << endl(this)
				<< "--ref.item->count;" << endl(this)
				<< pop(this) << "}" << endl(this)
				<< pop(this) << "}" << endl(this);

			outh << endl(this)
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

	void CEmitter::push() {
		contextStack.push_back(*context);
		context = &*(--contextStack.end());
	}

	void CEmitter::pop() {
		contextStack.pop_back();
		context = &*(--contextStack.end());
	}

	std::ostream &operator <<(std::ostream &stream, const CEmitter::endlt &value) {
		auto self = value.self;

		stream << "\n";
		util::leftpad(stream, self->context->indentation * self->context->indentationWidth, self->context->indentationChar);

		return stream;
	}

	CEmitter::endlt CEmitter::endl(CEmitter *self) {
		return {self};
	}

	std::ostream &operator <<(std::ostream &stream, const CEmitter::indentt &value) {
		auto self = value.self;

		self->context->indent();

		return stream;
	}

	CEmitter::indentt CEmitter::indent(CEmitter *self) {
		return {self};
	}

	std::ostream &operator <<(std::ostream &stream, const CEmitter::pusht &value) {
		auto self = value.self;

		self->push();

		return stream;
	}

	CEmitter::pusht CEmitter::push(CEmitter *self) {
		return {self};
	}

	std::ostream &operator <<(std::ostream &stream, const CEmitter::popt &value) {
		auto self = value.self;

		auto oldIndentation = self->context->indentation;
		self->pop();
		auto indentationDiff = oldIndentation - self->context->indentation;
		stream.seekp(-indentationDiff * self->context->indentationWidth, std::ios_base::cur);

		return stream;
	}

	CEmitter::popt CEmitter::pop(CEmitter *self) {
		return {self};
	}

	std::ostream &operator <<(std::ostream &stream, const CEmitter::pushindentt &value) {
		auto self = value.self;

		self->push();
		self->context->indent();

		return stream;
	}

	CEmitter::pushindentt CEmitter::pushindent(CEmitter *self) {
		return {self};
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

		outh << HEADER_H << endl(this);
		outc << HEADER_C << endl(this);

		auto moduleHash = typeNode->name->type;
		std::replace(moduleHash.begin(), moduleHash.end(), '.', '$');
		std::replace(moduleHash.begin(), moduleHash.end(), '<', '$');
		std::replace(moduleHash.begin(), moduleHash.end(), '>', '$');
		std::transform(moduleHash.begin(), moduleHash.end(), moduleHash.begin(), ::toupper);

		outh << endl(this)
			<< "#ifndef YAPL_MODULE_" << moduleHash << endl(this)
			<< "#define YAPL_MODULE_" << moduleHash << endl(this)
			<< "#include \"yapl/yapl.h\"" << endl(this);
		outc << endl(this)
			<< "#include \"" << includePath.relative(packageHeader) << "\"" << endl(this);

		if (auto classNode = dynamic_cast<const structure::ClassNode *>(typeNode)) {
			emit(classNode);
		}

		outh << endl(this)
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

		outh << endl(this)
			<< "struct " << classSymbolName << "$class {" << pushindent(this) << endl(this)
			<< "struct yapl$class $common;" << endl(this);
		placeVTable(classNode);
		outh << pop(this) << "};" << endl(this);

		outh << endl(this)
			<< "struct " << classSymbolName << " {" << pushindent(this) << endl(this)
			<< "struct " << classSymbolName << "$class *$class;" << endl(this);
		for (auto node : *classNode) {
			if (auto memberNode = dynamic_cast<structure::MemberNode *>(node)) {
				auto child = memberNode->get();

				if (auto methodMemberNode = dynamic_cast<structure::MethodMemberNode *>(child)) {

				} else if (auto variableMemberNode = dynamic_cast<structure::VariableMemberNode *>(child)) {
					if (memberNode->staticality == structure::MemberNode::Staticality::Dynamic) {
						outh << requestTypeRef(memberNode->type) << " " << getLast(memberNode->getName()) << ";" << endl(this);
					}
				}
			} else if (auto specialNode = dynamic_cast<structure::SpecialNode *>(node)) {
				emitInStruct(specialNode);
			}
		}
		outh << pop(this) << "};" << endl(this);

		outh << endl(this)
			<< "struct yapl$objectref " << classSymbolName << "$create();" << endl(this)
			<< endl(this);

		outc << endl(this)
			<< "struct yapl$objectref " << classSymbolName << "$create() {" << pushindent(this) << endl(this)
			<< "struct yapl$objectref result;" << endl(this)
			<< "yapl$objectref$init(result, malloc(sizeof(struct " << classSymbolName << ")));" << endl(this)
			<< "return result;" << endl(this)
			<< pop(this) << "}" << endl(this)
			<< endl(this);

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
			outh << ";" << endl(this);

			outc << endl(this);
			outc << requestTypeRef(memberNode->type) << " " << getFullMethodName(methodMemberNode);
			showArguments(outc, methodMemberNode->arguments, dynamic_cast<const structure::TypeNode *>(memberNode->getParent()));
			outc << " {" << endl(this);
			emit(methodMemberNode->body);
			outc << "}" << endl(this);
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
					outh << requestTypeRef(memberNode->type) << " (*" << getShortMethodName(methodMemberNode) << ")";
					showArguments(outh, methodMemberNode->arguments, classNode);
					outh << ";" << endl(this);
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
			outh << "char *buffer;" << endl(this);
			outh << "unsigned long length;" << endl(this);
		} else if (data == "yapl.Array") {
			// Template argument
			structure::TypeNameNode typeNameNode;
			typeNameNode.type = "T";
			outh << "" << requestTypeRef(&typeNameNode) << " *elements;" << endl(this);
			outh << "unsigned long count;" << endl(this);
		} else if (data == "yapl.Type") {

		} else {
			printf("%s\n", data.c_str());
			// TODO: error
		}
	}

	void CEmitter::emitInMethod(const structure::SpecialNode *specialNode) {
		auto data = specialNode->data;

		if (data == "yapl.String.constructor") {
			outh << "((yapl$String *)$this->target)->buffer = calloc(0, 1);" << endl(this);
		} else if (data == "yapl.String.constructor.string") {
			outh << endl(this);
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
