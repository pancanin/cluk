#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "memory.h"
#include "value.h"

typedef enum {
	OP_RETURN,
	OP_CONSTANT
} OpCode;

typedef struct {
	int line;
	int count;
} LineCount;


/// <summary>
/// A Chunk contains several bytecode instructions.
/// </summary>
typedef struct {
	size_t capacity;
	size_t count;
	/// <summary>
	/// Each instruction in the code is, most of the time, one byte.
	/// We are representing one byte with an unsigned 8 bit integer.
	/// </summary>
	uint8_t* code;

	/// <summary>
	/// Run-length encoded.
	/// </summary>
	int* lines;
	size_t linesCapacity;
	size_t linesCount;
	ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);

void writeChunk(Chunk* chunk, uint8_t byte, uint32_t line);

void freeChunk(Chunk* chunk);

int addConstant(Chunk* chunk, Value value);

int getLine(Chunk* chunk, int offset);

#endif