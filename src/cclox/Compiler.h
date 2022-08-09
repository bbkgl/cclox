#ifndef CCLOX_COMPILER_H
#define CCLOX_COMPILER_H

#include <memory>
#include <string_view>

namespace cclox {
    class Chunk;
    class Ast;
    class Parser;

    typedef std::unique_ptr<Ast> ASTUniquePtr;
    class Compiler {
    public:
        static Compiler& Get();

        std::unique_ptr<Chunk> Compile(std::string_view source);

        void CodeGen(ASTUniquePtr& root);
        void EndCompile();
    private:
        explicit Compiler();
        void RegisterRules();

        void PostDFSCodeGen(ASTUniquePtr& root);

        void EmitByte(uint8 byte);
        void EmitReturn();

        ASTUniquePtr PerformParseAst();
        ASTUniquePtr Expression();
        ASTUniquePtr Declaration();

        std::unique_ptr<Chunk> _currentCompilingChunk;
        std::unique_ptr<Parser> _currentParser;
    };
}

#endif //CCLOX_COMPILER_H
