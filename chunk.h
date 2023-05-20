#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "memory.h"
#include "value.h"

typedef enum {
	OP_RETURN,
	OP_CONSTANT,
	OP_CONSTANT_LONG
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

/// <summary>
/// Writes either an OP code or data to the byte code array.
/// Allows for variable size data to be written by specifying the number of bytes that will be written.
/// </summary>
/// <param name="chunk">The chunk of bytecode to which the data will be written.</param>
/// <param name="data">Could be an OP code or data up to 32 bits.</param>
/// <param name="line">The line number on which the OP or data is found. Used for debugging.</param>
/// <param name="numBytes">How many uint8_t (bytes) is the data?</param>
void writeChunk(Chunk* chunk, uint32_t data, uint32_t line, uint32_t numBytes);

void writeConstant(Chunk* chunk, Value value, uint32_t line);

/// <summary>
/// Stores 3 byte constant offset. If the user specified more than 256 constants the standard handling would fail, because it stores
/// the offset in a single byte.
/// </summary>
void writeConstantLong(Chunk* chunk, Value value, uint32_t line);

void freeChunk(Chunk* chunk);

int addConstant(Chunk* chunk, Value value);

int getLine(Chunk* chunk, int offset);

#endif