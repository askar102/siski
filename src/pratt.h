#include "ast.h"
#include "lexer.h"
#include "parser.h"

#include <cstdint>

class PrattParser : protected Parser {
private:



public:
    PrattParser(const std::vector<Token>& tokens)
        : Parser(tokens) {}

    ExpressionNode ParseExpression(size_t& curr_token_pos);

    /* Дает binging power оператора (типо его приоритет в таблице)*/
    int32_t lbp(TokenType type) const;
    char opChar(TokenType t) const;

    std::unique_ptr<ExpressionNode> nud();

    std::unique_ptr<ExpressionNode> led(std::unique_ptr<ExpressionNode> left);

    std::unique_ptr<ExpressionNode> parse(int32_t rbp);

};

