#ifndef CCLOX_COMMON_H
#define CCLOX_COMMON_H

#include <cstdint>
#include <cassert>

#define RUNTIME_ASSERT(expr) assert(expr)
#define STATIC_ASSERT(expr, msg) static_assert(expr, msg)

#define uint64 uint32_t
#define uint32 uint32_t
#define uint8  uint8_t
#define int64  int32_t
#define int32  int32_t
#define int8   int8_t

#define DEBUG_TRACE_EXECUTION

namespace cclox {
#define TOKEN_TABLE() \
    DEF_TOKEN(TOKEN_LEFT_PAREN) \
    DEF_TOKEN(TOKEN_RIGHT_PAREN) \
    DEF_TOKEN(TOKEN_LEFT_BRACE) \
    DEF_TOKEN(TOKEN_RIGHT_BRACE) \
    DEF_TOKEN(TOKEN_COMMA) \
    DEF_TOKEN(TOKEN_DOT) \
    DEF_TOKEN(TOKEN_MINUS) \
    DEF_TOKEN(TOKEN_PLUS) \
    DEF_TOKEN(TOKEN_SEMICOLON) \
    DEF_TOKEN(TOKEN_SLASH) \
    DEF_TOKEN(TOKEN_STAR)  \
    DEF_TOKEN(TOKEN_IDENTIFIER) \
    DEF_TOKEN(TOKEN_STRING) \
    DEF_TOKEN(TOKEN_NUMBER) \
    DEF_TOKEN(TOKEN_BANG) \
    DEF_TOKEN(TOKEN_BANG_EQUAL) \
    DEF_TOKEN(TOKEN_EQUAL) \
    DEF_TOKEN(TOKEN_EQUAL_EQUAL) \
    DEF_TOKEN(TOKEN_GREATER) \
    DEF_TOKEN(TOKEN_GREATER_EQUAL) \
    DEF_TOKEN(TOKEN_LESS) \
    DEF_TOKEN(TOKEN_LESS_EQUAL)  \
    DEF_TOKEN(TOKEN_AND) \
    DEF_TOKEN(TOKEN_CLASS) \
    DEF_TOKEN(TOKEN_ELSE) \
    DEF_TOKEN(TOKEN_FALSE) \
    DEF_TOKEN(TOKEN_FOR)  \
    DEF_TOKEN(TOKEN_FUN) \
    DEF_TOKEN(TOKEN_IF) \
    DEF_TOKEN(TOKEN_NIL) \
    DEF_TOKEN(TOKEN_OR) \
    DEF_TOKEN(TOKEN_PRINT) \
    DEF_TOKEN(TOKEN_RETURN) \
    DEF_TOKEN(TOKEN_SUPER) \
    DEF_TOKEN(TOKEN_THIS) \
    DEF_TOKEN(TOKEN_TRUE) \
    DEF_TOKEN(TOKEN_VAR) \
    DEF_TOKEN(TOKEN_WHILE)  \
    DEF_TOKEN(TOKEN_ERROR) \
    DEF_TOKEN(TOKEN_EOF)  \
    DEF_TOKEN(TOKEN_MAXNUM)  \


    enum TokenType : uint32 {
#define DEF_TOKEN(TOKEN_TYPE) TOKEN_TYPE,
        TOKEN_TABLE()
#undef DEF_TOKEN
    };

    inline const char* GetTokenTypeStr(TokenType type) {
        switch (type)
        {
#define DEF_TOKEN(TOKEN_TYPE) case TokenType::TOKEN_TYPE: \
                                  return #TOKEN_TYPE;
            TOKEN_TABLE()
#undef DEF_TOKEN
        }
        return nullptr;
    }

    typedef enum {
        OP_CONSTANT,
        OP_ADD,
        OP_SUBTRACT,
        OP_MULTIPLY,
        OP_DIVIDE,
        OP_NEGATE,
        OP_RETURN
    } OpCode;
}

#endif //CCLOX_COMMON_H