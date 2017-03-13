#pragma once

#include "Context.h"
#include "CodeStream.h"
#include "GCC.h"
#include "yaplc/emit/Emitter.h"
#include "yaplc/structure/PackageNode.h"
#include "yaplc/structure/TypeNode.h"
#include "yaplc/structure/ClassNode.h"
#include "yaplc/structure/MemberNode.h"
#include "yaplc/structure/MethodMemberNode.h"
#include "yaplc/structure/SpecialNode.h"
#include "yaplc/structure/VariableMemberNode.h"
#include <fstream>
#include <functional>

namespace yaplc { namespace cemit {
	class CEmitter : public emit::Emitter {
	private:
		GCC gcc;

		fs::path emitPath, includePath, sourcePath, objectPath, binPath;

		std::ofstream outstreamh, outstreamc;
		CodeStream outh, outc;

		struct FileEntry {
			fs::path header, source, object;
		};
		std::vector<FileEntry> files;

		std::vector<Context> contextStack;
		Context *context;

	public:
		CEmitter(const fs::path &emitPath);
		virtual ~CEmitter();

	private:
		void generateMain();

	protected:
		void push();
		void pop();

		virtual void emit(const structure::RootNode *rootNode);
		void emit(const structure::PackageNode *packageNode);
		void emit(const structure::TypeNode *typeNode);
		void emit(const structure::ClassNode *classNode);
		void emit(const structure::MemberNode *memberNode);
		void emit(const structure::VariableMemberNode *variableMemberNode);
		void emit(const structure::MethodMemberNode *methodMemberNode);
		void emit(const structure::ExpressionNode *expressionNode);

		void placeVTable(const structure::ClassNode *classNode, CodeStream &structMembers, CodeStream &initializator);
		void showArguments(CodeStream &stream, const structure::ArgumentsNode *argumentsNode, const structure::TypeNode *typeNode = nullptr, bool useNames = true);
		void include(CodeStream &stream, const structure::TypeNameNode *typeName);

		void emitInStruct(const structure::SpecialNode *specialNode);
		void emitInMethod(const structure::SpecialNode *specialNode);

		std::string requestTypeRef(const structure::TypeNameNode *typeNameNode, bool prependStruct = true);
		std::string requestType(const structure::TypeNameNode *typeNameNode, bool prependStruct = true);

	public:
		virtual void build();

	private:
		std::string convertName(const std::string &original);
		std::string getNotLast(const std::string &name);
		std::string getLast(const std::string &name);
		std::string getShortMethodName(const structure::MethodMemberNode *methodMemberNode);
		std::string getFullMethodName(const structure::MethodMemberNode *methodMemberNode);

		std::string shortName(const structure::TypeNameNode *typeNameNode) const;
	};
} }
