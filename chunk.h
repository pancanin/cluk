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
	/// Each instruction in the code has one byte for the OP code and one or more bytes for parameters.
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

/// <summary>
/// Writes a constant op code followed by an offset into the constants array. Adds the constant value into the constant array.
/// The offset value is up to 24 bits, so we could store 16777216 constants.
/// </summary>
/// <param name="chunk">Pointer to the chunk array.</param>
/// <param name="value">The value we want to store, could be an OP code or a constant value.</param>
/// <param name="line">The line number in the source code on which this constant is located.</param>
void writeConstant(Chunk* chunk, Value value, uint32_t line);

/// <summary>
/// Stores 3 byte constant offset. If the user specified more than 256 constants the standard handling would fail, because it stores
/// the offset in a single byte.
/// </summary>
void writeConstantLong(Chunk* chunk, Value value, uint32_t line);

/// <summary>
/// Reads a 3 byte sized constant offset from bytecode.
/// </summary>
/// <param name="offsetsOffset">The offset of the first byte of the offset value.</param>
/// <returns>The 24 bit offset value</returns>
uint32_t readConstantLongOffset(Chunk* chunk, uint32_t offsetsOffset);

void freeChunk(Chunk* chunk);

int addConstant(Chunk* chunk, Value value);

// TODO: This method has broken after introducing the long constants...
int getLine(Chunk* chunk, int offset);

#endif