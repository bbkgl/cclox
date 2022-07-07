
#ifndef CCLOX_TOKEN_H
#define CCLOX_TOKEN_H

#include "Common.h"

namespace cclox{
    class Token {
    public:
        explicit Token(TokenType type, const char* start, int length, int line);
        explicit Token(TokenType type);
        Token();

        const char* _start;
        TokenType _type : 8;
        uint32 _length : 8;
        uint32 _line : 16;
    };

    inline Token::Token(cclox::TokenType type)
            : _start(nullptr)
            , _type(type)
            , _length(0)
            , _line(0)
    {
    }

    inline Token::Token(cclox::TokenType type, const char* start, int length, int line)
            : _start(start)
            , _type(type)
            , _length(length)
            , _line(line)
    {
    }

    inline Token::Token()
        : _start(nullptr)
        , _type(TOKEN_ERROR)
        , _length(0)
        , _line(0)
    {

    }

    STATIC_ASSERT(sizeof(Token) <= 16, "Token size so large, should <= 16!");
}

#endif //CCLOX_TOKEN_H
