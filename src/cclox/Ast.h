
#ifndef CCLOX_AST_H
#define CCLOX_AST_H

#include <memory>
#include "Token.h"
#include "Value.h"

namespace cclox {
    class Chunk;

    class Ast {
    public:
        Ast(Token token);
        virtual ~Ast();

        std::unique_ptr<Ast> _lhs;
        std::unique_ptr<Ast> _rhs;

        virtual void CodeGen(Chunk* chunk);
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
}


#endif //CCLOX_AST_H
