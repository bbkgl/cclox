#ifndef CCLOX_COMPILER_H
#define CCLOX_COMPILER_H

#include <memory>
#include "Chunk.h"
#include "Parser.h"

namespace cclox {
    class Chunk;
    class Scanner;

    class Compiler {
    public:
        static Compiler& Get();

        std::unique_ptr<Chunk> Compile(const std::string& source);
        void EndCompile();
    private:
        explicit Compiler();
        void RegisterRules();

        void EmitByte(uint8 byte);
        void EmitBytes(uint8 byte1, uint8 byte2);
        void EmitConstant(Value value);
        void EmitReturn();

        Chunk::ConstantIndex MakeConstant(Value value);

        void MakeNumber();
        void MakeUnary();
        void MakeBinary();
        void Expression();
        void Grouping();

        std::unique_ptr<Chunk> _currentCompilingChunk;
        std::unique_ptr<Parser> _currentParser;
    };
}

#endif //CCLOX_COMPILER_H
