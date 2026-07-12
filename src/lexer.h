#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "log.h"

enum TokenType {
    // keywords
    IF, ELSE, 
    GOTO, LABEL,
    FUNC, RETURN,

    // another
    IDENTIFIER, INT_LITERAL, ARROW_OP, 
    EXTERN_C,

    // expression operators
    ASSIGN, PLUS, MINUS, STAR, SLASH, AMPERSAND,
    // condition operators
    EQUAL, LESS, GREATER, EQUAL_AND_LESS, EQUAL_AND_GREATER,


    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, SEMICOLON, COMMA, COLON,

    C_EOF, C_ERROR,
};


class Lexer {
public:
    Lexer() = default;

    void Tokenize(const std::string& content);
    const std::vector<std::pair<TokenType, std::string>>& GetTokens() const;

private:
    std::vector<std::pair<TokenType, std::string>> _tokens;

    void CheckDigit(const std::string& content, size_t& i);

    void CheckAlpha(const std::string& content, size_t& i);

    void CheckAloneOperators(const std::string& content, size_t& i);

    void CheckKeywords(const std::string& keyword);
};