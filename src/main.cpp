#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stdio.h>

#include "yaplc/compilingerror.h"
#include "yaplc/positionalerror.h"

#include "yaplc/parser/parsermanager.h"

#include "yaplc/structure/node.h"

#include "yaplc/util/markline.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::istreambuf_iterator;
using std::vector;
using std::ostream;
using std::exception;
using yaplc::CompilingError;
using yaplc::PositionalError;
using yaplc::parser::ParserManager;
using yaplc::structure::Node;
using yaplc::util::markline;


void showError(const std::string &code,
	const CompilingError *error, ostream &out) {
	out << "[";
		
	switch (error->type) {
	case CompilingError::Type::Syntax:
		out << "Syntax";
		break;
	case CompilingError::Type::Fatal:
		out << "Fatal";
		break;
	}
	
	out
		<< " Error]";
	
	if (auto positionalError = dynamic_cast<const PositionalError *>(error)) {
		if (positionalError->isDouble) {
			out
				<< "["
				<< positionalError->beginLineNumber + 1
				<< ":"
				<< positionalError->beginColumnNumber
				<< "-"
				<< positionalError->endLineNumber + 1
				<< ":"
				<< positionalError->endColumnNumber
				<< "]";
		} else {
			out
				<< "["
				<< positionalError->beginLineNumber + 1
				<< ":"
				<< positionalError->beginColumnNumber
				<< "]";
		}
		
		out
			<< ": "
			<< positionalError->message
			<< endl;
		
		if (positionalError->isDouble) {
			out
				<< markline(code,
					positionalError->beginLineNumber, positionalError->beginColumnNumber,
					positionalError->endLineNumber, positionalError->endColumnNumber)
				<< endl;
		} else {
			out
				<< markline(code,
					positionalError->beginLineNumber, positionalError->beginColumnNumber)
				<< endl;
		}
	} else {
		out
			<< ": "
			<< error->message
			<< endl;
	}
}

int main(int argc, char **argv) {
	try {
		ParserManager parser;
		
#ifdef INPUT_FILE
		ifstream ifs(INPUT_FILE);
#else
		ifstream ifs(argv[1]);
#endif
		string content((istreambuf_iterator<char>(ifs)),
						(istreambuf_iterator<char>()));
		
		vector<CompilingError *> errors;
		
		Node *node = parser.parse(content, errors);
		cout << node->show() << endl;
		delete node;
		
		for (auto error : errors) {
			showError(content, error, cout);
			delete error;
		}
	} catch (const exception &e) {
		cout << e.what() << endl;
	}
}
