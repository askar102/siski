#include "pratt.h"


int32_t PrattParser::lbp(TokenType type) const
{
    switch(type) {
        case TokenType::EQUAL:        
        case TokenType::NOT_EQUAL:      
        case TokenType::LESS:         
        case TokenType::GREATER:       
        case TokenType::LESS_EQUAL:    
        case TokenType::GREATER_EQUAL:
            return 5;
        case TokenType::PLUS:
        case TokenType::MINUS: return 10;
        case TokenType::STAR:
        case TokenType::SLASH: return 20;
        default: return 0;
    }
}

std::string PrattParser::opChar(TokenType t) const {
    switch (t) {
        case TokenType::PLUS:  return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR:  return "*";
        case TokenType::SLASH: return "/";
        default: return "?";
    }
}

std::unique_ptr<ExpressionNode> PrattParser::nud()
{
    Token t = Peek();
    switch (t.type) {
        case TokenType::NUMBER: {        
            Advance();
            return std::make_unique<NumberNode>(std::stoi(t.text));
        }

        case TokenType::MINUS: {
            Advance();
            return std::make_unique<UnaryNode>("-", parse(100));
        }

        case TokenType::IDENTIFIER: {
            Advance();
            return std::make_unique<VariableRefNode>(t.text);
        }

        case TokenType::LPAREN: {
            Advance();

            std::unique_ptr<ExpressionNode> inner = parse(0);

            if (Peek().type != TokenType::RPAREN) 
                throw std::runtime_error("Expected ')'");

            Advance();
            return inner;
        }

        default:
            throw std::runtime_error("Unexpected token in nud");
    }
}

std::unique_ptr<ExpressionNode> PrattParser::led(std::unique_ptr<ExpressionNode> left)
{
    Token t = Peek();
    int32_t bp = lbp(t.type);
    std::string op = opChar(t.type);
    Advance(); 
    std::unique_ptr<ExpressionNode> right = parse(bp);
    return std::make_unique<BinaryExpression>(std::move(left), op, std::move(right));
}

std::unique_ptr<ExpressionNode> PrattParser::parse(int32_t rbp) {
    std::unique_ptr<ExpressionNode> left = nud();
    while (rbp < lbp(Peek().type)) {
        left = led(std::move(left));
    }
    return left;
}

std::unique_ptr<ExpressionNode> PrattParser::ParseExpression(size_t& curr_token_pos)
{
    _curr_token_pos = curr_token_pos;
    std::unique_ptr<ExpressionNode> result = parse(0);
    curr_token_pos = _curr_token_pos; 
    return result;
}