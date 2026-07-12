#pragma once

#include "ast.h"
#include "lexer.h"
#include "parser.h"

#include <cstdint>

class PrattParser : protected Parser {
private:
    /* Дает binging power оператора (типо его приоритет в таблице)*/
    int32_t lbp(TokenType type) const;
    std::string opChar(TokenType t) const;

    std::unique_ptr<ExpressionNode> nud();

    std::unique_ptr<ExpressionNode> led(std::unique_ptr<ExpressionNode> left);

    std::unique_ptr<ExpressionNode> parse(int32_t rbp);
public:
    PrattParser(const std::vector<Token>& tokens)
        : Parser(tokens) {}

    std::unique_ptr<ExpressionNode> ParseExpression(size_t& curr_token_pos);
};

