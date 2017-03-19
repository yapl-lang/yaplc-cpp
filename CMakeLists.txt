cmake_minimum_required(VERSION 3.5)
project(yaplc)

set(CMAKE_CXX_STANDARD 11)

include_directories("src")

add_subdirectory(third-party/binstream)
add_subdirectory(third-party/fs)
add_subdirectory(third-party/machineid)

set(SOURCE_FILES
	src/main.cpp

	src/yaplc/cemit/CEmitter.cpp
	src/yaplc/cemit/CodeStream.cpp
	src/yaplc/cemit/GCC.cpp

	src/yaplc/emit/Emitter.cpp

	src/yaplc/parser/ArgumentsParser.cpp
	src/yaplc/parser/BracketExpressionParser.cpp
	src/yaplc/parser/ClassParser.cpp
	src/yaplc/parser/CodeOperatorParser.cpp
	src/yaplc/parser/CodeParser.cpp
	src/yaplc/parser/CodeVariableParser.cpp
	src/yaplc/parser/CommentsParser.cpp
	src/yaplc/parser/DoWhileOperatorParser.cpp
	src/yaplc/parser/ExpressionParser.cpp
	src/yaplc/parser/IfOperatorParser.cpp
	src/yaplc/parser/ImportParser.cpp
	src/yaplc/parser/MemberHeadParser.cpp
	src/yaplc/parser/MemberParser.cpp
	src/yaplc/parser/MethodMemberParser.cpp
	src/yaplc/parser/NullExpressionParser.cpp
	src/yaplc/parser/NumberExpressionParser.cpp
	src/yaplc/parser/OperatorMemberParser.cpp
	src/yaplc/parser/OperatorParser.cpp
	src/yaplc/parser/Parser.cpp
	src/yaplc/parser/ParserManager.cpp
	src/yaplc/parser/RootParser.cpp
	src/yaplc/parser/SpecialParser.cpp
	src/yaplc/parser/StringExpressionParser.cpp
	src/yaplc/parser/SyntaxError.cpp
	src/yaplc/parser/TypeExpressionParser.cpp
	src/yaplc/parser/TypeNameParser.cpp
	src/yaplc/parser/TypeParser.cpp
	src/yaplc/parser/VariableMemberParser.cpp
	src/yaplc/parser/WhileOperatorParser.cpp

	src/yaplc/process/LogicalError.cpp
	src/yaplc/process/Processor.cpp

	src/yaplc/structure/Childable.cpp
	src/yaplc/structure/Container.cpp
	src/yaplc/structure/Listable.cpp
	src/yaplc/structure/Node.cpp
	src/yaplc/structure/NodeFactory.cpp
	src/yaplc/structure/OperatorNode.cpp

	src/yaplc/util/getline.cpp
	src/yaplc/util/getlineandcolumn.cpp
	src/yaplc/util/getstringwidth.cpp
	src/yaplc/util/leftpad.cpp
	src/yaplc/util/map.cpp
	src/yaplc/util/markline.cpp
	src/yaplc/util/replace.cpp

	src/yaplc/Compiler.cpp
	src/yaplc/CompilingError.cpp
	src/yaplc/FatalError.cpp
	src/yaplc/PositionalError.cpp)

add_executable(yaplc ${SOURCE_FILES})
target_link_libraries(yaplc LINK_PUBLIC binstream fs machineid)
