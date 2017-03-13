#include "yapl/memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct yapl$memory$Chunk *firstChunk;
static struct yapl$memory$Chunk *currentChunk;
static unsigned long offset;

struct yapl$memory$Chunk *yapl$memory$createChunk(unsigned long size);

void yapl$memory$init() {
	firstChunk = currentChunk = yapl$memory$createChunk(0);
	offset = 0;
}

struct yapl$Object *yapl$memory$alloc(unsigned long size) {
	if (offset + size <= currentChunk->size) {
		struct yapl$Object *result = (struct yapl$Object *)(currentChunk->data + offset);
		offset += size;

		return result;
	}

	currentChunk->next = yapl$memory$createChunk(size);
	currentChunk = currentChunk->next;
	offset = size;

	return (struct yapl$Object *)currentChunk->data;
}

struct yapl$memory$Chunk *yapl$memory$createChunk(unsigned long size) {
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
