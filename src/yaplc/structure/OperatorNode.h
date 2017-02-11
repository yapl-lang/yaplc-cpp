#pragma once

#include "Node.h"

namespace yaplc { namespace structure {
	class OperatorNode : public Node {
	public:
		enum class Type {
			// Arithmetic
			Increment, // ++
			Decrement, // --
			Set, // =

			Add, // +
			Subtract, // -
			Multiply, // *
			Divide, // /

			AddSet, // +=
			SubtractSet, // -=
			MultiplySet, // *=
			DivideSet, // /=

			Module, // %

			// Comparison
			Equality, // ==
			Inequality, // !=

			More, // >
			Less, // <

			MoreEqual, // >=
			LessEqual, // <=
		};

	public:
		Type type;

		NODE_PROPS(
			NODE_PROP(type, {
				switch (type) {
				case Type::Increment:
					stream << "++";
					break;
				case Type::Decrement:
					stream << "--";
					break;
				case Type::Set:
					stream << "=";
					break;

				case Type::Add:
					stream << "+";
					break;
				case Type::Subtract:
					stream << "-";
					break;
				case Type::Multiply:
					stream << "*";
					break;
				case Type::Divide:
					stream << "/";
					break;

				case Type::AddSet:
					stream << "+=";
					break;
				case Type::SubtractSet:
					stream << "-=";
					break;
				case Type::MultiplySet:
					stream << "*=";
					break;
				case Type::DivideSet:
					stream << "/=";
					break;

				case Type::Module:
					stream << "%";
					break;

				case Type::Equality:
					stream << "==";
					break;
				case Type::Inequality:
					stream << "!=";
					break;

				case Type::More:
					stream << ">";
					break;
				case Type::Less:
					stream << "<";
					break;

				case Type::MoreEqual:
					stream << ">=";
					break;
				case Type::LessEqual:
					stream << "<=";
					break;
				}
			})
		)
	};
} }
