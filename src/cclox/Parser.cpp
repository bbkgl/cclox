#include "Parser.h"

namespace cclox {

    ParseRule Parser::StaticParseRules[TOKEN_MAXNUM] = {};

    Parser::Parser(std::unique_ptr<Scanner> scanner)
    : _scanner(std::move(scanner))
    , _panicMode(false)
    , _hadError(false)
    {
    }

    void Parser::Advance() {
        _previousToken = _currentToken;
        for (;;) {
            _currentToken = _scanner->GetNextToken();
            if (_currentToken._type != TOKEN_ERROR)
                break;

            ErrorAtCurrent(_currentToken._start);
        }
    }

    void Parser::ErrorAtCurrent(const char* message) {
        ErrorAt(_currentToken, message);
    }

    void Parser::ErrorAtLast(const char* message) {
        ErrorAt(_previousToken, message);
    }

    void Parser::ErrorAt(Token &token, const char *message) {
        if (_panicMode)
            return ;
        _panicMode = true;
        fprintf(stderr, "[Line: %d] Error", token._line);

        if (token._type == TOKEN_EOF) {
            fprintf(stderr, " at end");
        } else if (token._type == TOKEN_ERROR) {

        } else {
            fprintf(stderr, " at '%.*s'", token._length, token._start);
        }

        fprintf(stderr, ": %s\n", message);
        _hadError = true;
    }

    void Parser::Consume(TokenType expected, const char* message) {
        if (expected == _currentToken._type) {
            Advance();
            return ;
        }
        ErrorAtCurrent(message);
    }

    void Parser::ParsePrecedence(Precedence precedence) {
        Advance();

        const ParseFn& prefixRule = GetRule(_previousToken._type)->_prefixFn;
        if (prefixRule == nullptr)
        {
            ErrorAt(_previousToken, "Expect expression.");
            return;
        }

        prefixRule();

        while (precedence <= GetRule(_currentToken._type)->_precedence) {
            Advance();
            ParseFn infixRule = GetRule(_previousToken._type)->_infixFn;
            infixRule();
        }
    }

    ParseRule* Parser::GetRule(TokenType type) {
        return &StaticParseRules[type];
    }
}
