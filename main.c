#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  writeConstant(&chunk, 1.2, 1);
  writeConstant(&chunk, 3.4, 1);
  writeChunk(&chunk, OP_ADD, 1, 1);
  writeConstant(&chunk, 5.6, 1);
  writeChunk(&chunk, OP_DIVIDE, 1, 1);
  writeChunk(&chunk, OP_NEGATE, 1, 1);
  writeChunk(&chunk, OP_RETURN, 1, 1);

  //disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);
  freeChunk(&chunk);
  freeVM();
  return 0;
}
