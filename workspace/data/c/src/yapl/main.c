#include "yapl/main.h"
#include "yapl/memory.h"

int main(char **argc, int argv) {
	yapl$memory$init();

	return yapl$main(argc, argv);
}
