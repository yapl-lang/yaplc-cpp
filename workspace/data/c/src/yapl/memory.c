#include "yapl/memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct yapl$memory$Chunk *firstChunk;
static struct yapl$memory$Chunk *currentChunk;

struct yapl$memory$Chunk *yapl$memory$createChunk(unsigned long size);

void yapl$memory$init() {
	firstChunk = yapl$memory$createChunk(0);
}

struct yapl$Object *yapl$memory$alloc(unsigned long size) {
	if (currentChunk->offset + size <= currentChunk->size) {
		struct yapl$Object *result = (struct yapl$Object *)(currentChunk->data + currentChunk->offset);
		currentChunk->offset += size;

		return result;
	}

	currentChunk->next = yapl$memory$createChunk(size);
	currentChunk = currentChunk->next;

	return (struct yapl$Object *)currentChunk->data;
}

struct yapl$memory$Chunk *yapl$memory$createChunk(unsigned long size) {
	unsigned long chunkSize = size;
	if (chunkSize < DEFAULT_CHUNK_SIZE) {
		chunkSize = DEFAULT_CHUNK_SIZE;
	}

	struct yapl$memory$Chunk *chunk = malloc(sizeof(struct yapl$memory$Chunk) + chunkSize);
	chunk->next = NULL;
	chunk->offset = 0;
	chunk->size = chunkSize;
	memset(chunk->data, 0, chunkSize);

	return chunk;
}
