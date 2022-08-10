
#ifndef CCLOX_AST_H
#define CCLOX_AST_H

#include <memory>
#include "Token.h"
#include "Value.h"

namespace cclox {
    class Chunk;
    class Object;
    class ObjString;

    class Ast {
    public:
        explicit Ast(Token token);
        virtual ~Ast();

        std::unique_ptr<Ast> _lhs;
        std::unique_ptr<Ast> _rhs;

        virtual void CodeGen(Chunk* chunk);
        [[nodiscard]] TokenType GetTokenType();
    protected:
        Token _token;
    };

    class ExprAst : public Ast {
    public:
        explicit ExprAst(Token token);

        ~ExprAst() override;
    protected:
        virtual void EmitByte(Chunk* chunk, uint8 byte);
        virtual void EmitBytes(Chunk* chunk, uint8 byte1, uint8 byte2);
    };

    class LiteralExprAst : public ExprAst {
    public:
        explicit LiteralExprAst(Token token);
        ~LiteralExprAst() override;

        void CodeGen(Chunk* chunk) override;
    };

    class NumberExprAst : public ExprAst {
    public:
        explicit NumberExprAst(Token token ,Value value);

        ~NumberExprAst() override;

        void CodeGen(Chunk* chunk) override;
        Number GetNumber();
    private:
        Number _number;
    };

    class StringExprAst : public ExprAst {
    public:
        explicit StringExprAst(Token token);

        ~StringExprAst() override;

        void CodeGen(Chunk* chunk) override;
        ObjString* GetString();
    private:
        ObjString* _string;
    };

    class UnaryExprAst : public ExprAst {
    public:
        explicit UnaryExprAst(Token token);

        ~UnaryExprAst() override = default;

        void CodeGen(Chunk* chunk) override;
    };

    class BinaryExprAst : public ExprAst {
    public:
        explicit BinaryExprAst(Token token);
        ~BinaryExprAst() override = default;

        void CodeGen(Chunk* chunk) override;
    };

    class PrintStatementAst : public ExprAst {
    public:
        explicit PrintStatementAst(Token token);

        void CodeGen(Chunk* chunk) override;
    };

    class ExpressionStatementAst : public ExprAst {
    public:
        explicit ExpressionStatementAst(Token token);

        void CodeGen(Chunk* chunk) override;
    };

    class VarAst : public ExprAst {
    public:
        typedef enum : uint8 {
            GlobalVar = 0,
            ScopeVar = 1
        } VarType;
    public:
        explicit VarAst(Token token, VarType varType);

        void CodeGen(Chunk* chunk) override {};
    protected:
        VarType _varType;
    };

    class VarDeclarationAst : public VarAst {
    public:
        explicit VarDeclarationAst(Token token, VarType varType);

        void CodeGen(Chunk* chunk) override;
    };

    class VarAccessAst : public VarAst {
    public:
        explicit VarAccessAst(Token token, VarType varType);

        void CodeGen(Chunk* chunk) override;
    };
}


#endif //CCLOX_AST_H
