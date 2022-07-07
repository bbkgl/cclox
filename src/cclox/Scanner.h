#ifndef CCLOX_SCANNER_H
#define CCLOX_SCANNER_H

#include <string>
#include <vector>
#include "Common.h"
#include "Token.h"

namespace cclox{
    class Scanner {
    public:
        explicit Scanner(const std::string& source);

        Token GetNextToken();
        int GetLineNumber() const { return _currentLine; }
    private:
        bool IsAtEnd() { return _currentPos >= _source.length(); }
        char Advance();
        void SkipWhiteSpace();
        bool Match(char expected = '=');
        char Peek();
        char PeekNext();
        Token MakeToken(TokenType type) const;
        Token MakeError() const;

        Token MakeString();
        Token MakeNumber();
        Token MakeIdentify();
        Token DetermineIdentifyType();
        Token TryMakeKeyword(int restStart, int length, const char* rest, TokenType type) const;

        std::string _source;
        std::vector<Token> _tokens;

        int    _startPos;
        int    _currentPos;
        int    _currentLine;
    };


}

#endif //CCLOX_SCANNER_H
