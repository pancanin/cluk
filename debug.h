#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);

/// <summary>
/// Prints out human-readable information about a single bytecode instruction.
/// </summary>
/// <param name="chunk">The chunk that is being inspected</param>
/// <param name="offset">Offset into the Chunk - the index of the instruction in the stream of bytecode instructions.</param>
/// <returns>Returns the next offset. Useful because some instructions might be more than one byte long.</returns>
int disassembleInstruction(Chunk* chunk, int offset);

#endif // !DEBUG_H
