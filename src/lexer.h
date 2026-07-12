#pragma once

#include <string>
#include <fstream>
#include <vector>

#include "log.h"

enum TokenType {
    // keywords
    IF, ELSE, 
    GOTO, LABEL,
    RETURN,

    // another
    IDENTIFIER, INT_LITERAL, ARROW_OP, 
    EXTERN_C,

    // expression operators
    ASSIGN, PLUS, MINUS, STAR, SLASH, AMPERSAND,
    // condition operators
    EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,


    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, SEMICOLON, COMMA, COLON,

    C_EOF, C_ERROR,
};

struct Token {
    TokenType type;
    std::string text;
};


class Lexer {
public:
    Lexer() = default;

    void Tokenize(const std::string& content);
    const std::vector<Token>& GetTokens() const;

private:
    std::vector<Token> _tokens;

    void CheckDigit(const std::string& content, size_t& i);

    void CheckAlpha(const std::string& content, size_t& i);

    void CheckAloneOperators(const std::string& content, size_t& i);

    void CheckKeywords(const std::string& keyword);
};