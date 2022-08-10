#include "Ast.h"
#include "Chunk.h"
#include "ObjString.h"

namespace cclox {
    Ast::Ast(Token token)
    : _lhs(nullptr)
    , _rhs(nullptr)
    , _token(token)
    {
    }

    Ast::~Ast() {

    }

    void Ast::CodeGen(Chunk *chunk) {

    }

    TokenType Ast::GetTokenType() {
        return _token._type;
    }

    LiteralExprAst::LiteralExprAst(Token token) : ExprAst(token) {

    }

    LiteralExprAst::~LiteralExprAst() {

    }

    void LiteralExprAst::CodeGen(Chunk *chunk) {
        uint8 byte;
        switch (_token._type)
        {
            case TOKEN_FALSE:
            {
                byte = OP_FALSE;
                break;
            }
            case TOKEN_TRUE:
                byte = OP_TRUE;
                break;
            case TOKEN_NIL:
                byte = OP_NIL;
                break;
            default:
                return;
        }
        EmitByte(chunk, byte);
    }

    NumberExprAst::NumberExprAst(Token token, Value value)
    : ExprAst(token)
    , _number(value.as.number)
    {
    }


    NumberExprAst::~NumberExprAst() = default;

    void NumberExprAst::CodeGen(Chunk *chunk) {
        EmitByte(chunk, OP_CONSTANT);
        auto index = chunk->AddConstant(VAL_VAL(_number));
        EmitByte(chunk, index);
    }
    Number NumberExprAst::GetNumber() {
        return _number;
    }

    UnaryExprAst::UnaryExprAst(Token token)
    : ExprAst(token)
    {

    }

    void UnaryExprAst::CodeGen(Chunk *chunk) {
        switch (_token._type)
        {
            case TOKEN_MINUS:
                EmitByte(chunk, OP_NEGATE);
                break;
            case TOKEN_BANG:
                EmitByte(chunk, OP_NOT);
                break;
            default:
                return ;
        }
    }

    void BinaryExprAst::CodeGen(Chunk *chunk) {
        switch (_token._type)
        {
            case TOKEN_BANG_EQUAL:
                EmitBytes(chunk, OP_EQUAL, OP_NOT);
                break;
            case TOKEN_EQUAL_EQUAL:
                EmitByte(chunk, OP_EQUAL);
                break;
            case TOKEN_GREATER:
                EmitByte(chunk, OP_GREATER);
                break;
            case TOKEN_GREATER_EQUAL:
                EmitBytes(chunk, OP_LESS, OP_NOT);
                break;
            case TOKEN_LESS:
                EmitByte(chunk, OP_LESS);
            case TOKEN_LESS_EQUAL:
                EmitBytes(chunk, OP_GREATER, OP_NOT);
                break;
            case TOKEN_MINUS:
                EmitByte(chunk, OP_SUBTRACT);
                break;
            case TOKEN_PLUS:
                EmitByte(chunk, OP_ADD);
                break;
            case TOKEN_STAR:
                EmitByte(chunk, OP_MULTIPLY);
                break;
            case TOKEN_SLASH:
                EmitByte(chunk, OP_DIVIDE);
                break;
            default:
                return ;
        }
    }

    BinaryExprAst::BinaryExprAst(Token token)
    : ExprAst(token) {

    }

    ExprAst::~ExprAst() {

    }

    ExprAst::ExprAst(Token token)
    : Ast(token)
    {
    }

    void ExprAst::EmitByte(Chunk *chunk, uint8 byte) {
        chunk->WriteChunk(byte, _token._line);
    }

    void ExprAst::EmitBytes(Chunk *chunk, uint8_t byte1, uint8 byte2) {
        chunk->WriteChunk(byte1, _token._line);
        chunk->WriteChunk(byte2, _token._line);
    }

    StringExprAst::StringExprAst(Token token)
    : ExprAst(token)
    {
    }

    StringExprAst::~StringExprAst() {

    }

    void StringExprAst::CodeGen(Chunk *chunk) {
        EmitByte(chunk, OP_CONSTANT);
        Object* obj = ObjString::CreateString(std::string_view(_token._start + 1, _token._length - 2));
        auto index = chunk->AddConstant(OBJ_VAL(obj));
        EmitByte(chunk, index);
        _string = static_cast<ObjString*>(obj);
    }

    ObjString* StringExprAst::GetString() {
        return _string;
    }

    PrintStatementAst::PrintStatementAst(Token token)
    : ExprAst(token)
    {
    }

    void PrintStatementAst::CodeGen(Chunk *chunk)
    {
        EmitByte(chunk, OP_PRINT);
    }

    ExpressionStatementAst::ExpressionStatementAst(Token token)
    : ExprAst(token) {

    }

    void ExpressionStatementAst::CodeGen(Chunk *chunk) {
        EmitByte(chunk, OP_POP);
    }

    VarAst::VarAst(Token token, VarAst::VarType varType)
    : ExprAst(token)
    , _varType(varType) {

    }

    VarDeclarationAst::VarDeclarationAst(Token token, VarType varType)
    : VarAst(token, varType)
    {

    }

    void VarDeclarationAst::CodeGen(Chunk *chunk) {
        // Add variable to chunk table
        std::string_view symbol = std::string_view(_token._start, _token._length);
        auto globalIndex = cclox::Chunk::FindOrAddGlobal(symbol);

        switch (_varType)
        {
            case GlobalVar: {
                EmitBytes(chunk, OP_DEFINE_GLOBAL, globalIndex);
            }
            default:
                return ;
        }
    }

    VarAccessAst::VarAccessAst(Token token, VarType varType)
    : VarAst(token, varType) {
    }

    void VarAccessAst::CodeGen(Chunk *chunk) {
        // Add variable to chunk table
        std::string_view symbol = std::string_view(_token._start, _token._length);
        auto globalIndex = cclox::Chunk::FindOrAddGlobal(symbol);

        switch (_varType)
        {
            case GlobalVar: {
                EmitBytes(chunk, OP_GET_GLOBAL, globalIndex);
            }
            default:
                return ;
        }
    }
}