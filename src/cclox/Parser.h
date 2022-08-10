#ifndef CCLOX_PARSER_H
#define CCLOX_PARSER_H

#include <memory>
#include <functional>
#include "Token.h"
#include "Common.h"

namespace cclox {
    typedef enum {
        PREC_NONE,
        PREC_ASSIGNMENT,  // =
        PREC_OR,          // or
        PREC_AND,         // and
        PREC_EQUALITY,    // == !=
        PREC_COMPARISON,  // < > <= >=
        PREC_TERM,        // + -
        PREC_FACTOR,      // * /
        PREC_UNARY,       // ! -
        PREC_CALL,        // . ()
        PREC_PRIMARY
    } Precedence;

    class Ast;
    class Scanner;

    typedef std::unique_ptr<Ast> ASTUniquePtr;
    typedef std::function<ASTUniquePtr ()> PrefixParseFn;
    typedef std::function<ASTUniquePtr (ASTUniquePtr left)> InfixParseFn;
    typedef struct {
        PrefixParseFn _prefixFn;
        InfixParseFn _infixFn;
        Precedence _precedence;
    } ParseRule;

    class Parser {
    public:
        explicit Parser(std::unique_ptr<Scanner> scanner);

        void Advance();
        Token& Consume(TokenType expected, std::string_view message);
        static void RegisterRules(std::unique_ptr<Parser>& parser);

        [[nodiscard]] Token GetPreviousToken() const { return _previousToken; }
        [[nodiscard]] Token GetCurrentToken() const { return _currentToken; }

        [[nodiscard]] bool HadError() const { return _hadError; }

        static ParseRule StaticParseRules[TOKEN_MAXNUM];
        static ParseRule* GetRule(TokenType type);

        ASTUniquePtr Declaration();

        ASTUniquePtr ParsePrecedence(Precedence precedence);
        bool Match(TokenType type);
    private:
        void ErrorAtCurrent(std::string_view message);
        void ErrorAtLast(std::string_view message);
        void ErrorAt(Token& token, std::string_view message);

        // statement
        ASTUniquePtr VarDeclaration();
        ASTUniquePtr Statement();
        ASTUniquePtr PrintStatement();
        ASTUniquePtr ExpressionStatement();
        void Synchronize();

        // parse sth
        ASTUniquePtr ParseVariable(std::string_view errorMsg);

        // infix && prefix function
        ASTUniquePtr Expression();
        ASTUniquePtr Variable();
        ASTUniquePtr Number();
        ASTUniquePtr Unary();
        ASTUniquePtr Group();
        ASTUniquePtr Binary(ASTUniquePtr left);
        ASTUniquePtr Literal();
        ASTUniquePtr String();



        // check
        [[nodiscard]] bool Check(TokenType type) const;

        Token _currentToken;
        Token _previousToken;

        bool  _hadError;
        bool  _panicMode;

        std::unique_ptr<Scanner> _scanner;
    };
}


#endif //CCLOX_PARSER_H
