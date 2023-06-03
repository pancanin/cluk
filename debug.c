#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
	printf("== %s ==\n", name);

	for (size_t offset = 0; offset < chunk->count;) {
		offset = disassembleInstruction(chunk, offset);
	}
}
int disassembleInstruction(Chunk* chunk, int offset) {
  printf("%04d ", offset);
  if (offset > 0 &&
    getLine(chunk, offset) == getLine(chunk, offset - 1)) {
    printf("   | ");
  }
  else {
    printf("%4d ", getLine(chunk, offset));
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  case OP_CONSTANT:
	  return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_CONSTANT_LONG:
    return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
}

static int simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk,
	int offset) {
	uint8_t constantIdx = chunk->code[offset + 1];
	printf("%-16s %4d '", name, constantIdx);
	printValue(chunk->constants.values[constantIdx]);
	printf("'\n");
	return offset + 2;
}

static int constantLongInstruction(const char* name, Chunk* chunk, uint32_t offset) {
  uint32_t idx = readConstantLongOffset(chunk, offset + 1);
  printf("%-16s %4d '", name, idx);
  printValue(chunk->constants.values[idx]);
  printf("'\n");
  return offset + 4;
}
