#include "yapl/memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yapl$memory$Allocator$init(struct yapl$memory$Allocator *allocator) {
	allocator->firstChunk = allocator->currentChunk = yapl$memory$Allocator$createChunk(0);
	allocator->offset = 0;
}

struct yapl$memory$Chunk *yapl$memory$Allocator$createChunk(unsigned long size) {
	unsigned long chunkSize = size;
	if (chunkSize < DEFAULT_CHUNK_SIZE) {
		chunkSize = DEFAULT_CHUNK_SIZE;
	}

	struct yapl$memory$Chunk *chunk = malloc(sizeof(struct yapl$memory$Chunk) + chunkSize);
	chunk->next = NULL;
	chunk->size = chunkSize;
	memset(chunk->data, 0, chunkSize);

	return chunk;
}

void *yapl$memory$Allocator$alloc(struct yapl$memory$Allocator *allocator, unsigned long size) {
	if (allocator->offset + size <= allocator->currentChunk->size) {
		struct yapl$Object *result = (void *)(allocator->currentChunk->data + allocator->offset);
		allocator->offset += size;

		return result;
	}

	allocator->currentChunk->next = yapl$memory$Allocator$createChunk(size);
	allocator->currentChunk = allocator->currentChunk->next;
	allocator->offset = size;

	return (void *)allocator->currentChunk->data;
}


struct yapl$memory$Allocator staticMemoryAllocator;
struct yapl$memory$Allocator objectMemoryAllocator;

void yapl$memory$init() {
	yapl$memory$Allocator$init(&staticMemoryAllocator);
	yapl$memory$Allocator$init(&objectMemoryAllocator);
}

void *yapl$memory$allocStatic(unsigned long size) {
	return yapl$memory$Allocator$alloc(&staticMemoryAllocator, size);
}
struct yapl$Object *yapl$memory$allocObject(unsigned long size) {
	return (struct yapl$Object *)yapl$memory$Allocator$alloc(&objectMemoryAllocator, size);
}


