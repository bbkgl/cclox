#ifndef CCLOX_CHUNK_H
#define CCLOX_CHUNK_H
#include <vector>

#include "VirtualMachine.h"
#include "Common.h"
#include "Value.h"

namespace cclox {
    class Chunk {
    public:
        typedef std::vector<uint8>   CodeArray;
        typedef std::vector<uint32>  LineArray;
        typedef uint8                ConstantIndex;

        explicit Chunk();

        Chunk(Chunk&& inChunk);

        void WriteChunk(const uint8 byte, uint32 line);
        ConstantIndex AddConstant(Value value);

        CodeArray GetChunkCodes() const {
            return _code;
        }

        ValueArray GetConstantValues() const {
            return _constants;
        }

        LineArray GetLines() const {
            return _lines;
        }
    private:
        uint8* GetCodePointer();

        CodeArray    _code;
        ValueArray   _constants;
        LineArray    _lines;

        friend class VirtualMachine;
    };
}

#endif //CCLOX_CHUNK_H
