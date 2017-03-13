#ifndef YAPL_MEMORY
#define YAPL_MEMORY

#include "yapl/Object.h"

#define DEFAULT_CHUNK_SIZE 4096

struct yapl$memory$Chunk {
	struct yapl$memory$Chunk *next;
	unsigned long size;
	unsigned char data[0];
};

struct yapl$memory$Allocator {
	struct yapl$memory$Chunk *firstChunk;
	struct yapl$memory$Chunk *currentChunk;
	unsigned long offset;
};

void yapl$memory$Allocator$init(struct yapl$memory$Allocator *allocator);
struct yapl$memory$Chunk *yapl$memory$Allocator$createChunk(unsigned long size);
void *yapl$memory$Allocator$alloc(struct yapl$memory$Allocator *allocator, unsigned long size);

void yapl$memory$init();
void *yapl$memory$allocStatic(unsigned long size);
struct yapl$Object *yapl$memory$allocObject(unsigned long size);

#endif
