#include "parser.h"

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

ExpressionNode Parser::ParseExpression() {}
