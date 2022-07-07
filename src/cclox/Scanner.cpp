//
// Created by 秋水 on 2022/6/19.
//

#include <cstring>
#include <cassert>
#include "Scanner.h"

namespace cclox {
    Scanner::Scanner(const std::string& source)
    : _source(source)
    , _currentLine(1)
    , _currentPos(0)
    , _startPos(0)
    {

    }

    Token Scanner::GetNextToken() {
        SkipWhiteSpace();
        _startPos = _currentPos;
        if (IsAtEnd())
            return Token(TOKEN_EOF);

        char lastChar = Advance();
        if (std::isdigit(lastChar))
            return MakeNumber();

        if (std::isalpha(lastChar))
            return MakeIdentify();

        switch (lastChar) {
            case '(': return MakeToken(TOKEN_LEFT_PAREN);
            case ')': return MakeToken(TOKEN_RIGHT_PAREN);
            case '{': return MakeToken(TOKEN_LEFT_BRACE);
            case '}': return MakeToken(TOKEN_RIGHT_BRACE);
            case ';': return MakeToken(TOKEN_SEMICOLON);
            case ',': return MakeToken(TOKEN_COMMA);
            case '.': return MakeToken(TOKEN_DOT);
            case '-': return MakeToken(TOKEN_MINUS);
            case '+': return MakeToken(TOKEN_PLUS);
            case '/': return MakeToken(TOKEN_SLASH);
            case '*': return MakeToken(TOKEN_STAR);
            case '!':
                return MakeToken(Match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
            case '=':
                return MakeToken(Match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
            case '<':
                return MakeToken(Match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
            case '>':
                return MakeToken(Match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
            case '"':
                return MakeString();
        }
        return MakeError();
    }

    char Scanner::Advance() {
        _currentPos++;
        return _source[_currentPos - 1];
    }

    Token Scanner::MakeToken(TokenType type) const {
        const char* lexPtr = _startPos + _source.data();
        return Token(type, lexPtr, _currentPos - _startPos, _currentLine);
    }

    Token Scanner::MakeString() {
        while (Peek() != '"' && !IsAtEnd()) {
            if (Peek() == '\n')
                ++_currentLine;
            Advance();
        }

        Advance();  // skip "
        return MakeToken(TOKEN_STRING);
    }

    bool Scanner::Match(char expected) {
        if (IsAtEnd())
            return false;
        if (_source[_currentPos] != expected)
            return false;
        Advance();
        return true;
    }

    void Scanner::SkipWhiteSpace() {
        if (IsAtEnd())
            return ;

        for (;;)
        {
            char lastChar = Peek();
            switch (lastChar) {
                case ' ':
                case '\t':
                case '\r':
                    Advance();
                    break;
                case '\n':
                    Advance();
                    ++_currentLine;
                    break;
                case '/': {
                    if (PeekNext() == '/') {
                        Advance();
                        while (!IsAtEnd() && Peek() != '\n')
                            Advance();
                    }
                    else
                        return ;
                }
                default:
                    return ;
            }
        }
    }

    char Scanner::Peek() {
        return _source[_currentPos];
    }

    char Scanner::PeekNext() {
        if (IsAtEnd())
            return '\0';
        return _source[_currentPos + 1];
    }

    Token Scanner::MakeNumber() {
        while (std::isdigit(Peek()) && !IsAtEnd()) {
            Advance();
        }

        if (Peek() == '.' && std::isdigit(PeekNext())) {
            Advance();
            while (std::isdigit(Peek()) && !IsAtEnd()) {
                Advance();
            }
        }

        return MakeToken(TOKEN_NUMBER);
    }

    Token Scanner::MakeIdentify() {
        while (std::isalnum(Peek()))
            Advance();
        return DetermineIdentifyType();
    }

    Token Scanner::DetermineIdentifyType() {
        char startChar = _source[_startPos];
        switch (startChar) {
            case 'a':
                return TryMakeKeyword(1, 2, "nd", TOKEN_AND);
            case 'c':
                return TryMakeKeyword(1, 4, "lass", TOKEN_CLASS);
            case 'e':
                return TryMakeKeyword(1, 3, "lse", TOKEN_ELSE);
            case 'f': {
                if (_currentPos - _startPos <= 1)
                    break;
                char nextChar = _source[_startPos + 1];
                switch (nextChar) {
                    case 'a':
                        return TryMakeKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return TryMakeKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return TryMakeKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            case 'i':
                return TryMakeKeyword(1, 1, "f", TOKEN_IF);
            case 'n':
                return TryMakeKeyword(1, 2, "il", TOKEN_NIL);
            case 'o':
                return TryMakeKeyword(1, 1, "r", TOKEN_OR);
            case 'p':
                return TryMakeKeyword(1, 4, "rint", TOKEN_PRINT);
            case 'r':
                return TryMakeKeyword(1, 5, "eturn", TOKEN_RETURN);
            case 's':
                return TryMakeKeyword(1, 4, "uper", TOKEN_SUPER);
            case 't': {
                if (_currentPos - _startPos <= 1)
                    break;

                char nextChar = _source[_startPos + 1];
                switch (nextChar) {
                    case 'h':
                        return TryMakeKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return TryMakeKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            case 'v':
                return TryMakeKeyword(1, 2, "ar", TOKEN_VAR);
            case 'w':
                return TryMakeKeyword(1, 4, "hile", TOKEN_WHILE);
            default:
                return MakeToken(TOKEN_IDENTIFIER);
        }
    }

    Token Scanner::TryMakeKeyword(int restStart, int restLength, const char *rest, TokenType type) const {
        RUNTIME_ASSERT(restLength == std::strlen(rest));

        if (_currentPos - _startPos == restStart + restLength) {
            if (std::memcmp(rest, _source.data() + restStart, restLength) == 0)
                return MakeToken(type);
        }
        return MakeToken(TOKEN_IDENTIFIER);
    }

    Token Scanner::MakeError() const {
        return Token(TOKEN_ERROR, "Error!", -1, _currentLine);
    }

}
