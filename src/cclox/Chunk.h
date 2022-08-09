#ifndef CCLOX_CHUNK_H
#define CCLOX_CHUNK_H
#include <vector>

#include "VirtualMachine.h"
#include "Common.h"
#include "Value.h"

namespace cclox {
    class Chunk {
    public:
        typedef std::vector<uint8>            CodeArray;
        typedef std::vector<uint32>           LineArray;
        typedef std::vector<std::string>      SymbolArray;
        typedef uint8                         ConstantIndex;

        explicit Chunk();

        Chunk(Chunk&& inChunk) noexcept ;

        void WriteChunk(uint8 byte, uint32 line);


        ConstantIndex AddConstant(Value value);

        static ConstantIndex AddGlobal(std::string_view symbol);

        [[nodiscard]] CodeArray GetChunkCodes() const {
            return _code;
        }

        [[nodiscard]] ValueArray GetConstantValues() const {
            return _constants;
        }

        [[nodiscard]] LineArray GetLines() const {
            return _lines;
        }

        [[nodiscard]] static SymbolArray GetGlobals() {
            return _sGlobals;
        }
    private:
        uint8* GetCodePointer();

        CodeArray    _code;
        ValueArray   _constants;
        LineArray    _lines;

        static SymbolArray _sGlobals;
        friend class VirtualMachine;
    };
}

#endif //CCLOX_CHUNK_H
