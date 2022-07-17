#ifndef CCLOX_COMPILER_H
#define CCLOX_COMPILER_H

#include <memory>
#include <string_view>
#include "Chunk.h"
#include "Parser.h"

namespace cclox {
    class Compiler {
    public:
        static Compiler& Get();

        std::unique_ptr<Chunk> Compile(std::string_view source);

        void CodeGen(ASTRef& root);
        void EndCompile();
    private:
        explicit Compiler();
        void RegisterRules();

        void PostDFSCodeGen(ASTRef& root);

        void EmitByte(uint8 byte);
        void EmitReturn();

        ASTRef PerformParseAst();
        ASTRef Expression();

        std::unique_ptr<Chunk> _currentCompilingChunk;
        std::unique_ptr<Parser> _currentParser;
    };
}

#endif //CCLOX_COMPILER_H
