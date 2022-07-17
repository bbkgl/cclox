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
        void Consume(TokenType expected, std::string_view message);
        static void RegisterRules(std::unique_ptr<Parser>& parser);

        Token GetPreviousToken() const { return _previousToken; }
        Token GetCurrentToken() const { return _currentToken; }

        bool HadError() const { return _hadError; }

        static ParseRule StaticParseRules[TOKEN_MAXNUM];
        static ParseRule* GetRule(TokenType type);


        ASTUniquePtr ParsePrecedence(Precedence precedence);
    private:
        void ErrorAtCurrent(std::string_view message);
        void ErrorAtLast(std::string_view message);
        void ErrorAt(Token& token, std::string_view message);

        // infix && prefix function
        ASTUniquePtr Expression();
        ASTUniquePtr Number();
        ASTUniquePtr Unary();
        ASTUniquePtr Group();
        ASTUniquePtr Binary(ASTUniquePtr left);
        ASTUniquePtr Literal();

        Token _currentToken;
        Token _previousToken;

        bool  _hadError;
        bool  _panicMode;

        std::unique_ptr<Scanner> _scanner;
    };
}


#endif //CCLOX_PARSER_H
