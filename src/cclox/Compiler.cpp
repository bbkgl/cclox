#include "Scanner.h"
#include "Compiler.h"

namespace cclox {
    Compiler& Compiler::Get() {
        static Compiler staticCompiler;
        return staticCompiler;
    }

    void Compiler::RegisterRules() {
        ParseFn lambdaGrouping = [this]() { Grouping(); };
        ParseFn lambdaBinary = [this]() { MakeBinary(); };
        ParseFn lambdaUnary = [this]() { MakeUnary(); };
        ParseFn lambdaNumber = [this]() { MakeNumber(); };

        Parser::StaticParseRules[TOKEN_LEFT_PAREN]    = {lambdaGrouping, nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_RIGHT_PAREN]   = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_LEFT_BRACE]    = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_RIGHT_BRACE]   = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_COMMA]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_DOT]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_MINUS]         = {lambdaUnary,    lambdaBinary, PREC_TERM};
        Parser::StaticParseRules[TOKEN_PLUS]          = {nullptr,     lambdaBinary, PREC_TERM};
        Parser::StaticParseRules[TOKEN_SEMICOLON]     = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_SLASH]         = {nullptr,     lambdaBinary, PREC_FACTOR};
        Parser::StaticParseRules[TOKEN_STAR]          = {nullptr,     lambdaBinary, PREC_FACTOR};
        Parser::StaticParseRules[TOKEN_BANG]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_BANG_EQUAL]    = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_EQUAL]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_EQUAL_EQUAL]   = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_GREATER]       = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_GREATER_EQUAL] = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_LESS]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_LESS_EQUAL]    = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_IDENTIFIER]    = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_STRING]        = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_NUMBER]        = {lambdaNumber,   nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_AND]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_CLASS]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_ELSE]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_FALSE]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_FOR]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_FUN]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_IF]            = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_NIL]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_OR]            = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_PRINT]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_RETURN]        = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_SUPER]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_THIS]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_TRUE]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_VAR]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_WHILE]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_ERROR]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_EOF]           = {nullptr,     nullptr,   PREC_NONE};
    }

    Compiler::Compiler()
    {
        RegisterRules();
    }

    std::unique_ptr<Chunk> Compiler::Compile(const std::string &source) {
        auto scanner = std::make_unique<Scanner>(source);
        _currentParser = std::make_unique<Parser>(std::move(scanner));
        _currentCompilingChunk = std::make_unique<Chunk>();

        _currentParser->Advance();
        Expression();

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

    void Compiler::EmitBytes(uint8_t byte1, uint8 byte2) {
        EmitByte(byte1);
        EmitByte(byte2);
    }

    void Compiler::EmitConstant(Value value) {
        EmitBytes(OP_CONSTANT, MakeConstant(value));
    }

    Chunk::ConstantIndex Compiler::MakeConstant(Value value) {
        Chunk::ConstantIndex constantIndex = _currentCompilingChunk->AddConstant(value);
        return constantIndex;
    }

    void Compiler::MakeNumber() {
        Value value = std::strtod(_currentParser->GetPreviousToken()._start, nullptr);
        EmitConstant(value);
    }

    void Compiler::Expression() {
        _currentParser->ParsePrecedence(PREC_ASSIGNMENT);
    }

    void Compiler::Grouping() {
        Expression();
        _currentParser->Consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
    }

    void Compiler::MakeUnary() {
        Token preToken = _currentParser->GetPreviousToken();
        TokenType operatorType = preToken._type;

        _currentParser->ParsePrecedence(PREC_UNARY);

        // Emit byte
        switch (operatorType)
        {
            case TOKEN_MINUS:
                EmitByte(OP_NEGATE);
                break;
            default:
                return ;
        }
    }

    void Compiler::MakeBinary() {
        TokenType operatorType = _currentParser->GetPreviousToken()._type;
        ParseRule* rule = _currentParser->GetRule(operatorType);
        _currentParser->ParsePrecedence((Precedence)(rule->_precedence + 1));

        switch (operatorType) {
            case TOKEN_PLUS:          EmitByte(OP_ADD); break;
            case TOKEN_MINUS:         EmitByte(OP_SUBTRACT); break;
            case TOKEN_STAR:          EmitByte(OP_MULTIPLY); break;
            case TOKEN_SLASH:         EmitByte(OP_DIVIDE); break;
            default: return; // Unreachable.
        }
    }
}

