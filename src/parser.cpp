#include "parser.h"

void Parser::ParseProgram() {}

Token Parser::Peek() const 
{
    return _tokens[_curr_token_pos];
}

Token Parser::Advance() 
{
    return _tokens[_curr_token_pos++];    
} 

bool Parser::Check(TokenType type)
{
    return true ? Peek().type == type : false;
} 

bool Parser::Match(TokenType type) 
{
    if (Peek().type == type) 
    {
        ++_curr_token_pos;
        return true;
    }
    else 
    {
        return false;
    }
}

Token Parser::Except(TokenType type) 
{
    if (Peek().type == type) 
    {
        return _tokens[_curr_token_pos++];
    }
    else
    {
        LOG("ERROR: excepted cheto tam %s", Peek().text);
    }
}

std::unique_ptr<ExpressionNode> Parser::ParseExpression()
{
    return pratt.ParseExpression(_curr_token_pos);
}

std::unique_ptr<ReturnStatement> Parser::ParseReturnStmt()
{
    Advance();
    
    std::unique_ptr<ExpressionNode> expr = nullptr;

    if (!Match(TokenType::SEMICOLON)) {
        expr = ParseExpression();
    }

    Except(TokenType::SEMICOLON);

    return std::make_unique<ReturnStatement>(std::move(expr));
}


