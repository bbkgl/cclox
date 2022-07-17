#include "Scanner.h"
#include "Compiler.h"

namespace cclox {
    Compiler& Compiler::Get() {
        static Compiler staticCompiler;
        return staticCompiler;
    }

    Compiler::Compiler()
    {
    }

    std::unique_ptr<Chunk> Compiler::Compile(const std::string &source) {
        auto scanner = std::make_unique<Scanner>(source);
        _currentParser = std::make_unique<Parser>(std::move(scanner));
        RegisterRules();
        _currentCompilingChunk = std::make_unique<Chunk>();

        _currentParser->Advance();
        ASTRef ast = PerformParseAst();
        CodeGen(ast);

        EndCompile();

        if (_currentParser->HadError())
            return nullptr;
        return std::move(_currentCompilingChunk);
    }

    void Compiler::EmitByte(uint8_t byte) {
        Token preToken = _currentParser->GetPreviousToken();
        _currentCompilingChunk->WriteChunk(byte, preToken._line);
    }

    void Compiler::EmitReturn() {
        EmitByte(OP_RETURN);
    }

    void Compiler::EndCompile() {
        EmitReturn();
    }

    ASTRef Compiler::PerformParseAst() {
        return Expression();
    }

    ASTRef Compiler::Expression() {
        ASTRef expressionAst = _currentParser->ParsePrecedence(PREC_ASSIGNMENT);
        return expressionAst;
    }

    void Compiler::RegisterRules() {
        _currentParser->RegisterRules(_currentParser);
    }

    void Compiler::CodeGen(ASTRef& root) {
        PostDFSCodeGen(root);
    }

    void Compiler::PostDFSCodeGen(ASTRef& root) {
        if (!root)
            return ;
        PostDFSCodeGen(root->_lhs);
        PostDFSCodeGen(root->_rhs);
        root->CodeGen(_currentCompilingChunk.get());
    }
}
