#include "Debug.h"
#include "Value.h"

namespace cclox {
    static int SimpleInstruction(const char* name, int offset) {
        printf("%s\n", name);
        return offset + 1;
    }

    static int ConstantInstruction(const char* name, int offset, const Chunk::CodeArray& code, const ValueArray& constants) {
        Chunk::ConstantIndex constantIndex = code[offset + 1];
        printf("%-16s %4d '", name, constantIndex);
        PrintValue(constants[constantIndex]);
        printf("'\n");
        return offset + 2;
    }

    void DisassembleChunk(const char* name, Chunk* chunk) {
        printf("== %s ==\n", name);

        Chunk::CodeArray code = chunk->GetChunkCodes();
        ValueArray constants = chunk->GetConstantValues();
        for (size_t offset = 0; offset < code.size();)
        {
            offset = DisassembleInstruction(chunk, offset);
        }
    }

    size_t DisassembleInstruction(const Chunk* const chunk, const size_t offset) {
        printf("%04zu ", offset);

        auto code = chunk->GetChunkCodes();
        ValueArray constants = chunk->GetConstantValues();
        auto lines = chunk->GetLines();
        uint8 instruction = code[offset];
        if (offset > 0 && lines[offset] == lines[offset - 1])
            printf("   | ");
        else
            printf("%4d ", lines[offset]);

        switch (instruction) {
            case OP_RETURN:
                return SimpleInstruction("OP_RETURN", offset);
            case OP_CONSTANT:
                return ConstantInstruction("OP_CONSTANT", offset, code, constants);
            case OP_NEGATE:
                return SimpleInstruction("OP_NEGATE", offset);
            case OP_ADD:
                return SimpleInstruction("OP_ADD", offset);
            case OP_SUBTRACT:
                return SimpleInstruction("OP_SUBTRACT", offset);
            case OP_MULTIPLY:
                return SimpleInstruction("OP_MULTIPLY", offset);
            case OP_DIVIDE:
                return SimpleInstruction("OP_DIVIDE", offset);
            default:
                printf("Unknown opcode %d\n", instruction);
                return offset + 1;
        }
    }


}