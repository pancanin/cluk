#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "memory.h"

typedef enum {
	OP_RETURN
} OpCode;


// TODO: Create a impl file and possibly move this definition there.
typedef struct {
	size_t capacity;
	size_t count;
	uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk) {
	assert(chunk);
	chunk->capacity = 0;
	chunk->count = 0;
	chunk->code = NULL;
}

void writeChunk(Chunk* chunk, uint8_t byte) {
	assert(chunk);

	if (chunk->count + 1 > chunk->capacity) {
		size_t oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(chunk->capacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;
	chunk->count++;
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	initChunk(chunk);
}

#endif