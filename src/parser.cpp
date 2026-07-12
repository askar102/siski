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

bool Parser::Match(TokenType type) {
}