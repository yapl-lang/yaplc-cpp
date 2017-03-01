#pragma once

#include "yaplc/emit/Emitter.h"
#include "yaplc/structure/PackageNode.h"
#include "yaplc/structure/TypeNode.h"
#include "yaplc/structure/ClassNode.h"
#include "yaplc/structure/MemberNode.h"
#include "yaplc/structure/MethodMemberNode.h"
#include "yaplc/structure/SpecialNode.h"
#include <fstream>

namespace yaplc { namespace cemit {
	class CEmitter : public emit::Emitter {
	private:
		fs::path emitPath, includePath, sourcePath, objectPath, binPath;

		std::ofstream outh, outc;

		struct FileEntry {
			fs::path header, source, object;
		};
		std::vector<FileEntry> files;

	public:
		CEmitter(const fs::path &emitPath);
		virtual ~CEmitter();

	private:
		void generateMain();

	protected:
		virtual void emit(const structure::RootNode *rootNode);
		void emit(const structure::PackageNode *packageNode);
		void emit(const structure::TypeNode *typeNode);
		void emit(const structure::ClassNode *classNode);
		void emit(const structure::MemberNode *memberNode);

		void placeVTable(const structure::ClassNode *classNode);

		void emitInStruct(const structure::SpecialNode *specialNode);

		std::string requestType(const structure::TypeNameNode *typeNameNode);

	public:
		virtual void build();

	private:
		static std::string convertName(const std::string &original);
	};
} }
