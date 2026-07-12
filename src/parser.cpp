#include "parser.h"

TokenType Parser::Peek() const 
{
    return _tokens[_curr_token_pos].first;
}

TokenType Parser::Advance() 
{
    return _tokens[_curr_token_pos++].first;    
} 

bool Parser::Check(TokenType type)
{
    return Peek()
} 

