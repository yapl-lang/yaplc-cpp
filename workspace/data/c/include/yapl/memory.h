#ifndef YAPL_MEMORY
#define YAPL_MEMORY

#include "yapl/Object.h"
#include "yapl/reference.h"

#define ALLOCATOR_DEFAULT_CHUNK_SIZE 4096
#define ALLOCATOR_REFERENCES_COUNT 64

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

struct yapl$memory$ReferencesChunk {
	struct yapl$memory$ReferencesChunk *next;
	struct yapl$Reference references[ALLOCATOR_REFERENCES_COUNT];
};

struct yapl$memory$ReferencesAllocator {
	struct yapl$memory$ReferencesChunk *firstChunk;
	struct yapl$memory$ReferencesChunk *currentChunk;
	unsigned long offset;
};

void yapl$memory$Allocator$init(struct yapl$memory$Allocator *allocator);
struct yapl$memory$Chunk *yapl$memory$Allocator$createChunk(unsigned long size);
void *yapl$memory$Allocator$alloc(struct yapl$memory$Allocator *allocator, unsigned long size);

void yapl$memory$ReferencesAllocator$init(struct yapl$memory$ReferencesAllocator *allocator);
struct yapl$memory$ReferencesChunk *yapl$memory$ReferencesAllocator$createChunk();
struct yapl$Reference *yapl$memory$ReferencesAllocator$alloc(struct yapl$memory$ReferencesAllocator *allocator);

void yapl$memory$init();
void *yapl$memory$allocStatic(unsigned long size);
struct yapl$Object *yapl$memory$allocObject(unsigned long size);
struct yapl$Reference *yapl$memory$allocReference();

void yapl$memory$gc();

#endif
