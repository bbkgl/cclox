#ifndef CCLOX_PARSER_H
#define CCLOX_PARSER_H

#include <memory>
#include <functional>
#include "Common.h"
#include "Token.h"
#include "Scanner.h"

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

    typedef std::function<void()> ParseFn;
    typedef struct {
        ParseFn _prefixFn;
        ParseFn _infixFn;
        Precedence _precedence;
    } ParseRule;

    class Parser {
    public:
        explicit Parser(std::unique_ptr<Scanner> scanner);

        void Advance();
        void Consume(TokenType expected, const char* message);

        Token GetPreviousToken() const { return _previousToken; }
        Token GetCurrentToken() const { return _currentToken; }

        bool HadError() const { return _hadError; }

        static ParseRule StaticParseRules[TOKEN_MAXNUM];
        ParseRule* GetRule(TokenType type);


        void ParsePrecedence(Precedence precedence);
    private:
        void ErrorAtCurrent(const char* message);
        void ErrorAtLast(const char* message);
        void ErrorAt(Token& token, const char* message);

        Token _currentToken;
        Token _previousToken;

        bool  _hadError;
        bool  _panicMode;

        std::unique_ptr<Scanner> _scanner;
    };
}


#endif //CCLOX_PARSER_H
