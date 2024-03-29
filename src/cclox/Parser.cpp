#include <charconv>
#include "Scanner.h"
#include "Parser.h"
#include "Ast.h"

namespace cclox {

    ParseRule Parser::StaticParseRules[TOKEN_MAXNUM] = {};

    Parser::Parser(std::unique_ptr<Scanner> scanner)
    : _scanner(std::move(scanner))
    , _panicMode(false)
    , _hadError(false)
    {
    }

    void Parser::RegisterRules(std::unique_ptr<Parser>& parser)
    {
        PrefixParseFn lambdaGrouping = [&parser]() { return parser->Group(); };
        InfixParseFn lambdaBinary = [&parser](ASTUniquePtr left) { return parser->Binary(std::move(left)); };
        PrefixParseFn lambdaUnary = [&parser]() { return parser->Unary(); };
        PrefixParseFn lambdaNumber = [&parser]() { return parser->Number(); };
        PrefixParseFn lambdaLiteral = [&parser]() { return parser->Literal(); };
        PrefixParseFn lambdaString = [&parser]() { return parser->String(); };
        PrefixParseFn lambdaVariable = [&parser]() { return parser->Variable(); };

        Parser::StaticParseRules[TOKEN_LEFT_PAREN]    = {lambdaGrouping, nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_RIGHT_PAREN]   = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_LEFT_BRACE]    = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_RIGHT_BRACE]   = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_COMMA]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_DOT]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_MINUS]         = {lambdaUnary,    lambdaBinary, PREC_TERM};
        Parser::StaticParseRules[TOKEN_PLUS]          = {nullptr,     lambdaBinary, PREC_TERM};
        Parser::StaticParseRules[TOKEN_SEMICOLON]     = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_SLASH]         = {nullptr,     lambdaBinary, PREC_FACTOR};
        Parser::StaticParseRules[TOKEN_STAR]          = {nullptr,     lambdaBinary, PREC_FACTOR};
        Parser::StaticParseRules[TOKEN_BANG]          = {lambdaUnary,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_BANG_EQUAL]    = {nullptr,     lambdaBinary,   PREC_EQUALITY};
        Parser::StaticParseRules[TOKEN_EQUAL]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_EQUAL_EQUAL]   = {nullptr,     lambdaBinary,   PREC_EQUALITY};
        Parser::StaticParseRules[TOKEN_GREATER]       = {nullptr,     lambdaBinary,   PREC_COMPARISON};
        Parser::StaticParseRules[TOKEN_GREATER_EQUAL] = {nullptr,     lambdaBinary,   PREC_COMPARISON};
        Parser::StaticParseRules[TOKEN_LESS]          = {nullptr,     lambdaBinary,   PREC_COMPARISON};
        Parser::StaticParseRules[TOKEN_LESS_EQUAL]    = {nullptr,     lambdaBinary,   PREC_COMPARISON};
        Parser::StaticParseRules[TOKEN_IDENTIFIER]    = {lambdaVariable,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_STRING]        = {lambdaString,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_NUMBER]        = {lambdaNumber,   nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_AND]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_CLASS]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_ELSE]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_FALSE]         = {lambdaLiteral,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_FOR]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_FUN]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_IF]            = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_NIL]           = {lambdaLiteral,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_OR]            = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_PRINT]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_RETURN]        = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_SUPER]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_THIS]          = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_TRUE]          = {lambdaLiteral,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_VAR]           = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_WHILE]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_ERROR]         = {nullptr,     nullptr,   PREC_NONE};
        Parser::StaticParseRules[TOKEN_EOF]           = {nullptr,     nullptr,   PREC_NONE};
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

    void Parser::ErrorAtCurrent(std::string_view message) {
        ErrorAt(_currentToken, message);
    }

    void Parser::ErrorAtLast(std::string_view message) {
        ErrorAt(_previousToken, message);
    }

    void Parser::ErrorAt(Token &token, std::string_view message) {
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

        fprintf(stderr, ": %s\n", message.data());
        _hadError = true;
    }

    Token& Parser::Consume(TokenType expected, std::string_view message) {
        if (expected == _currentToken._type) {
            Advance();
            return _previousToken;
        }
        ErrorAtCurrent(message);
    }

    ASTUniquePtr Parser::ParsePrecedence(Precedence precedence) {
        Advance();

        const PrefixParseFn & prefixRule = GetRule(_previousToken._type)->_prefixFn;
        if (prefixRule == nullptr)
        {
            ErrorAt(_previousToken, "Expect expression.");
            return nullptr;
        }

        ASTUniquePtr left = prefixRule();

        while (precedence <= GetRule(_currentToken._type)->_precedence) {
            Advance();
            InfixParseFn infixRule = GetRule(_previousToken._type)->_infixFn;
            left = infixRule(std::move(left));
        }
        return left;
    }

    ParseRule* Parser::GetRule(TokenType type) {
        return &StaticParseRules[type];
    }

    ASTUniquePtr Parser::Number() {
        const char* str = _previousToken._start;
        cclox::Number value = std::strtod(str, nullptr);
        ASTUniquePtr numberAst = std::make_unique<NumberExprAst>(_previousToken, NUMBER_VAL(value));
        return std::move(numberAst);
    }

    ASTUniquePtr Parser::Unary() {
        ASTUniquePtr unaryAst = std::make_unique<UnaryExprAst>(_previousToken);
        unaryAst->_rhs = ParsePrecedence(PREC_UNARY);
        return unaryAst;
    }

    ASTUniquePtr Parser::Group() {
        ASTUniquePtr groupedAst = Expression();
        Consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression");
        return groupedAst;
    }

    ASTUniquePtr Parser::Expression() {
        return ParsePrecedence(PREC_ASSIGNMENT);
    }

    ASTUniquePtr Parser::Binary(ASTUniquePtr left) {
        auto binAst = std::make_unique<BinaryExprAst>(_previousToken);
        TokenType binOpType = _previousToken._type;
        ParseRule *rule = GetRule(binOpType);

        binAst->_lhs = std::move(left);
        binAst->_rhs = ParsePrecedence((Precedence)(rule->_precedence + 1));
        return binAst;
    }

    ASTUniquePtr Parser::Literal() {
        auto literalAst = std::make_unique<LiteralExprAst>(_previousToken);
        return literalAst;
    }

    ASTUniquePtr Parser::String() {
        ASTUniquePtr strAst = std::make_unique<StringExprAst>(_previousToken);
        return std::move(strAst);
    }

    bool Parser::Check(TokenType type) const {
        return _currentToken._type == type;
    }

    bool Parser::Match(TokenType type) {
        if (!Check(type)) return false;
        Advance();
        return true;
    }

    ASTUniquePtr Parser::VarDeclaration() {
        ASTUniquePtr varDeclAst = ParseVariable("Expect variable name.");

        // default value: nil
        if (Match(TOKEN_EQUAL))
            varDeclAst->_rhs = Expression();
//        else //
//            varDeclAst->_rhs = std::make_unique<ExprAst>(TOKEN_NIL);

        Consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");
        return varDeclAst;
    }

    ASTUniquePtr Parser::Statement() {
        if (Match(TOKEN_PRINT))
            return PrintStatement();
        else
            return ExpressionStatement();
    }

    ASTUniquePtr Parser::PrintStatement() {
        ASTUniquePtr printAst = std::make_unique<PrintStatementAst>(_previousToken);
        printAst->_rhs = Expression();
        Consume(TOKEN_SEMICOLON, "Expect a ';' after print value");
        return printAst;
    }

    ASTUniquePtr Parser::ExpressionStatement() {
        auto exprAst = Expression();
        Consume(TOKEN_SEMICOLON, "Expect a ';' after a statement");
        ASTUniquePtr exprStatementAst = std::make_unique<ExpressionStatementAst>(_currentToken);
        exprStatementAst->_rhs = std::move(exprAst);
        return exprStatementAst;
    }

    ASTUniquePtr Parser::Declaration() {
        ASTUniquePtr declAst = nullptr;
        if (Match(TOKEN_VAR))
            declAst = VarDeclaration();
        else
            declAst = Statement();
        if (_panicMode)
            Synchronize();
        return declAst;
    }

    void Parser::Synchronize() {
        _panicMode = false;

        while (_currentToken._type != TOKEN_EOF)
        {
            if (_previousToken._type == TOKEN_SEMICOLON)
                return ;
            switch (_currentToken._type) {
                case TOKEN_PRINT:
                case TOKEN_CLASS:
                case TOKEN_FUN:
                case TOKEN_VAR:
                case TOKEN_WHILE:
                case TOKEN_IF:
                case TOKEN_FOR:
                case TOKEN_RETURN:
                    return ;

                default:
                    // do nothing
                    ;
            }
            Advance();
        }
    }

    ASTUniquePtr Parser::ParseVariable(std::string_view errorMsg) {
        Token& identifyToken = Consume(TOKEN_IDENTIFIER, errorMsg);
        return std::make_unique<VarDeclarationAst>(identifyToken, VarDeclarationAst::GlobalVar);
    }

    ASTUniquePtr Parser::Variable() {
        ASTUniquePtr varAccessAst = std::make_unique<VarAccessAst>(_previousToken, VarAst::GlobalVar);
        return varAccessAst;
    }
}
