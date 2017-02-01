#include "markline.h"
#include "getline.h"
#include "leftpad.h"
#include "getstringwidth.h"

#include <sstream>

namespace yaplc { namespace util {
	std::string markline(const std::string &string,
		unsigned long line, unsigned long column) {
		std::stringstream stream;
		
		std::string currentLine = getline(string, line);
		stream << currentLine;
		stream << currentLine << std::endl;
		stream << leftpad("|", getstringwidth(currentLine, column));
		
		return stream.str();
	}
	
	std::string markline(const std::string &string,
		unsigned long line1, unsigned long column1,
		unsigned long line2, unsigned long column2) {
		if (line1 > line2) {
			unsigned long tmp;
			
			tmp = line1; line1 = line2; line2 = tmp;
			tmp = column1; column1 = column2; column2 = tmp;
		}
		
		std::stringstream stream;
		
		stream << leftpad("|", getstringwidth(getline(string, line1), column1)) << std::endl;
		
		for (unsigned long i = line1; i <= line2; ++i) {
			stream << getline(string, i) << std::endl;
		}
		
		stream << leftpad("|", getstringwidth(getline(string, line2), column2));
		
		return stream.str();
	}
} }
