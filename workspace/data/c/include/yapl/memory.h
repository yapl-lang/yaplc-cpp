#ifndef YAPL_MEMORY
#define YAPL_MEMORY

#include "yapl/Object.h"

#define DEFAULT_CHUNK_SIZE 4096

struct yapl$memory$Chunk {
	struct yapl$memory$Chunk *next;
	unsigned long size;
	unsigned char data[0];
};

void yapl$memory$init();
struct yapl$Object *yapl$memory$alloc(unsigned long size);

#endif
