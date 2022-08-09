#include "Debug.h"
#include "Value.h"

namespace cclox {
    static size_t SimpleInstruction(std::string_view name, size_t offset) {
        printf("%s\n", name.data());
        return offset + 1;
    }

    static size_t ConstantInstruction(std::string_view name, size_t offset, const Chunk::CodeArray& code, const ValueArray& constants) {
        Chunk::ConstantIndex constantIndex = code[offset + 1];
        printf("%-16s %4d '", name.data(), constantIndex);
        PrintValue(constants[constantIndex]);
        printf("'\n");
        return offset + 2;
    }

    static size_t GlobalDefineInstruction(std::string_view name, size_t offset, const Chunk::CodeArray& code, const Chunk::SymbolArray & globals) {
        Chunk::ConstantIndex globalIndex = code[offset + 1];
        printf("%-16s %4d '", name.data(), globalIndex);
        PrintValue(std::string_view(globals[globalIndex]));
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
            case OP_DEFINE_GLOBAL:
                return GlobalDefineInstruction(GetOpTypeStr(static_cast<OpCode>(instruction)), offset, code, Chunk::GetGlobals());
            case OP_CONSTANT:
                return ConstantInstruction(GetOpTypeStr(static_cast<OpCode>(instruction)), offset, code, constants);
            case OP_FALSE:
            case OP_TRUE:
            case OP_NIL:
            case OP_RETURN:
            case OP_NOT:
            case OP_NEGATE:
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_EQUAL:
            case OP_LESS:
            case OP_GREATER:
            case OP_DIVIDE:
            case OP_PRINT:
            case OP_POP:
                return SimpleInstruction(GetOpTypeStr(static_cast<OpCode>(instruction)), offset);
            default:
                printf("Unknown opcode %d\n", instruction);
                return offset + 1;
        }
    }


}