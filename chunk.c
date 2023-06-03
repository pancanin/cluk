#include "chunk.h"

static void fillWithZeroes(int* arr, int count, int startIdx) {
	for (int i = startIdx; i < count; i++) {
		arr[i] = 0;
	}
}

void initChunk(Chunk* chunk) {
	assert(chunk);
	chunk->capacity = 0;
	chunk->count = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	chunk->linesCapacity = 0;
	chunk->linesCount = 0;
	initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint32_t byte, uint32_t line, uint32_t numBytes) {
	assert(chunk);
	assert(numBytes <= 4);

	if (chunk->count + numBytes > chunk->capacity) {
		size_t oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(chunk->capacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}

	if (line > chunk->linesCapacity) {
		size_t oldCapacity = chunk->linesCapacity;
		chunk->linesCapacity = GROW_CAPACITY(chunk->linesCapacity);
		chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->linesCapacity);
		fillWithZeroes(chunk->lines, chunk->linesCapacity, oldCapacity);
	}

	if (numBytes == 1) {
		chunk->code[chunk->count] = (uint8_t)byte;
		chunk->count++;
	}
	else {
		// 1. Shift the integer value so that the most significant bit is at the left-most position.
		uint8_t shiftSize = (sizeof(uint32_t) - numBytes) * 8;
		byte = byte << shiftSize;
		memcpy(&chunk->code[chunk->count], &byte, numBytes);
		chunk->count += numBytes;
	}

	// Run-length encode the line numbers.
	if (chunk->lines[line - 1] == 0) {
		chunk->linesCount++;
	}
	chunk->lines[line - 1]++;
}

void writeConstant(Chunk* chunk, Value value, uint32_t line) {
	int constantOffset = addConstant(chunk, value);
	writeChunk(chunk, OP_CONSTANT, line, 1);
	writeChunk(chunk, constantOffset, line, 1);
}

void writeConstantLong(Chunk* chunk, Value value, uint32_t line) {
	int constantOffset = addConstant(chunk, value);
	writeChunk(chunk, OP_CONSTANT_LONG, line, 1);
	writeChunk(chunk, constantOffset, line, 3);
}

uint32_t readConstantLongOffset(Chunk* chunk, uint32_t offsetsOffset) {
	// Copy three bytes from chunk memory to an int
	uint32_t offset = 0;
	memcpy(&offset, chunk->code + offsetsOffset, 3);
	uint8_t shiftSize = (sizeof(uint32_t) - 3) * 8;
	offset = offset >> shiftSize;
	return offset;
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int offset) {
	int counter = 0;
	int instructionNumber = offset + 1;

	for (int lineMinusOne = 0; lineMinusOne < chunk->linesCount; lineMinusOne++) {
		int instructionCountForLine = chunk->lines[lineMinusOne];
		counter += instructionCountForLine;

		if (counter >= instructionNumber) {
			return lineMinusOne + 1;
		}
	}

	return -1;
}
