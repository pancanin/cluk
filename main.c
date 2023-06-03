#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  initChunk(&chunk);

  writeConstantLong(&chunk, 2.1, 2);
  writeConstantLong(&chunk, 1.61, 2);
  writeConstantLong(&chunk, 0.45, 3);

  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
  return 0;
}
