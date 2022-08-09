#include "Scanner.h"
#include "Compiler.h"
#include "Chunk.h"
#include "Parser.h"
#include "Ast.h"

namespace cclox {
    Compiler& Compiler::Get() {
        static Compiler staticCompiler;
        return staticCompiler;
    }

    Compiler::Compiler()
    {
    }

    std::unique_ptr<Chunk> Compiler::Compile(std::string_view source) {
        auto scanner = std::make_unique<Scanner>(source);
        _currentParser = std::make_unique<Parser>(std::move(scanner));
        RegisterRules();
        _currentCompilingChunk = std::make_unique<Chunk>();

        _currentParser->Advance();
        while (!_currentParser->Match(TOKEN_EOF))
        {
            ASTUniquePtr ast = PerformParseAst();
            CodeGen(ast);
        }

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

    ASTUniquePtr Compiler::PerformParseAst() {
//        return Expression();
        return Declaration();
    }

    ASTUniquePtr Compiler::Expression() {
        ASTUniquePtr expressionAst = _currentParser->ParsePrecedence(PREC_ASSIGNMENT);
        return expressionAst;
    }

    ASTUniquePtr Compiler::Declaration() {
        ASTUniquePtr expressionAst = _currentParser->Declaration();
        return expressionAst;
    }

    void Compiler::RegisterRules() {
        _currentParser->RegisterRules(_currentParser);
    }

    void Compiler::CodeGen(ASTUniquePtr& root) {
        PostDFSCodeGen(root);
    }

    void Compiler::PostDFSCodeGen(ASTUniquePtr& root) {
        if (!root)
            return ;
        PostDFSCodeGen(root->_lhs);
        PostDFSCodeGen(root->_rhs);
        root->CodeGen(_currentCompilingChunk.get());
    }
}
