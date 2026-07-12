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
    return Peek().type == type;
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

Token Parser::Expect(TokenType type) 
{
    if (Peek().type == type) 
    {
        return _tokens[_curr_token_pos++];
    }
    else
    {
        throw std::runtime_error("Expected token, got: " + Peek().text);
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

    Expect(TokenType::SEMICOLON);

    return std::make_unique<ReturnStatement>(std::move(expr));
}


std::unique_ptr<GotoStatement> Parser::ParseGotoStmt()
{
    Advance();

    Token t = Expect(TokenType::IDENTIFIER);
    return std::make_unique<GotoStatement>(t.text);
}

std::unique_ptr<LabelStatement> Parser::ParseLabelStmt()
{
    Advance();

    Token t = Expect(TokenType::IDENTIFIER);

    Expect(TokenType::COLON);

    return std::make_unique<LabelStatement>(t.text);
}

std::unique_ptr<VariableAssignNode> Parser::ParseVariableAssign()
{
    std::string var_name = Advance().text;

    std::unique_ptr<ExpressionNode> expr = nullptr;

    if(Match(TokenType::ASSIGN)) 
    {
        expr = ParseExpression();
    }
    else
    {
        throw std::runtime_error("Variable assign error");
    }

    Expect(SEMICOLON);
    return std::make_unique<VariableAssignNode>(var_name, std::move(expr));
}

std::unique_ptr<FunctionCallNode> Parser::ParseFunctionCallStmt()
{
    std::string func_name = Advance().text;
    std::vector<std::unique_ptr<ExpressionNode>> call_args;

    Expect(TokenType::LPAREN);

    if (!Check(TokenType::RPAREN))
    {
        call_args.push_back(ParseExpression());
        while (Match(TokenType::COMMA))
        {
            call_args.push_back(ParseExpression());
        }  
    }

    Expect(TokenType::RPAREN);
    Expect(TokenType::SEMICOLON);

    return std::make_unique<FunctionCallNode>(func_name, std::move(call_args));
}

