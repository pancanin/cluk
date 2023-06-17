#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  initChunk(&chunk);

  writeConstantLong(&chunk, 2.1, 1);
  writeConstantLong(&chunk, 1.61, 1);
  writeConstantLong(&chunk, 0.45, 2);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
  return 0;
}
