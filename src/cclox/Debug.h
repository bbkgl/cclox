#ifndef CCLOX_DEBUG_H
#define CCLOX_DEBUG_H

#include <string>
#include "Common.h"
#include "Chunk.h"

namespace cclox {
    void DisassembleChunk(const char* name, Chunk* chunk);
    size_t DisassembleInstruction(const Chunk* const chunk, const size_t offset);
}


#endif //CCLOX_DEBUG_H
