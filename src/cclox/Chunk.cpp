#include "Chunk.h"

namespace cclox
{
    Chunk::Chunk()
    {

    }

    void Chunk::WriteChunk(const uint8_t byte, uint32 line) {
        _code.push_back(byte);
        _lines.push_back(line);
    }

    Chunk::ConstantIndex Chunk::AddConstant(Value value) {
        auto index = static_cast<ConstantIndex>(_constants.size());
        _constants.push_back(value);
        RUNTIME_ASSERT(_constants.size() <= UINT8_MAX);
        return index;
    }

    uint8 *Chunk::GetCodePointer() {
        return _code.data();
    }

    Chunk::Chunk(Chunk &&inChunk) {
        _code = std::move(inChunk._code);
        _constants = std::move(inChunk._constants);
        _lines = std::move(inChunk._lines);
    }
}
