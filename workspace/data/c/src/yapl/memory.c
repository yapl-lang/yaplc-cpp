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
	if (chunkSize < ALLOCATOR_DEFAULT_CHUNK_SIZE) {
		chunkSize = ALLOCATOR_DEFAULT_CHUNK_SIZE;
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


void yapl$memory$ReferencesAllocator$init(struct yapl$memory$ReferencesAllocator *allocator) {
	allocator->firstChunk = allocator->currentChunk = yapl$memory$ReferencesAllocator$createChunk(0);
	allocator->offset = 0;
}

struct yapl$memory$ReferencesChunk *yapl$memory$ReferencesAllocator$createChunk() {
	struct yapl$memory$ReferencesChunk *chunk = malloc(sizeof(struct yapl$memory$ReferencesChunk));
	chunk->next = NULL;
	memset(chunk->references, 0, sizeof(chunk->references));

	return chunk;
}

struct yapl$Reference *yapl$memory$ReferencesAllocator$alloc(struct yapl$memory$ReferencesAllocator *allocator) {
	if (allocator->offset == ALLOCATOR_REFERENCES_COUNT) {
		for (unsigned long i = 0; i < ALLOCATOR_REFERENCES_COUNT; ++i) {
			if (allocator->currentChunk->references[i].target == NULL) {
				return &allocator->currentChunk->references[i];
			}
		}

		allocator->currentChunk = yapl$memory$ReferencesAllocator$createChunk();
		allocator->currentChunk->next = allocator->currentChunk;
	}

	struct yapl$Reference *reference = &allocator->currentChunk->references[allocator->offset];
	allocator->offset++;

	return reference;
}


static struct yapl$memory$Allocator staticMemoryAllocator;
static struct yapl$memory$Allocator objectMemoryAllocator;
static struct yapl$memory$ReferencesAllocator referencesMemoryAllocator;

void yapl$memory$init() {
	yapl$memory$Allocator$init(&staticMemoryAllocator);
	yapl$memory$Allocator$init(&objectMemoryAllocator);
	yapl$memory$ReferencesAllocator$init(&referencesMemoryAllocator);
}

void *yapl$memory$allocStatic(unsigned long size) {
	return yapl$memory$Allocator$alloc(&staticMemoryAllocator, size);
}
struct yapl$Object *yapl$memory$allocObject(unsigned long size) {
	return (struct yapl$Object *)yapl$memory$Allocator$alloc(&objectMemoryAllocator, size);
}

struct yapl$Reference *yapl$memory$allocReference() {

}

void yapl$memory$gc() {
	{
		struct yapl$memory$ReferencesChunk **currentChunk = &(referencesMemoryAllocator.firstChunk);

		while (*currentChunk) {
			unsigned char clear = 1;

			for (unsigned long i = 0; i < ALLOCATOR_REFERENCES_COUNT; ++i) {
				if ((*currentChunk)->references[i].target != NULL) {
					clear = 0;
					break;
				}
			}

			if (clear) {
				free(*currentChunk);
				*currentChunk = (*currentChunk)->next;
			}

			currentChunk = &(*currentChunk)->next;
		}
	}
}


