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

bool Parser::CheckNext(TokenType type)
{
    if (Peek().type != TokenType::C_EOF)
    {
        return _tokens[_curr_token_pos+1].type == type;
    }

    return false;
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

std::unique_ptr<StatementNode> Parser::ParseStatement()
{
    if (Check(TokenType::IF)) {
        return ParseIfStmt();
    }

    if (Check(TokenType::GOTO)) {
        return ParseGotoStmt();
    }

    if (Check(TokenType::LABEL)) {
        return ParseLabelStmt();
    }

    if (Check(TokenType::RETURN)) {
        return ParseReturnStmt();
    }

    if (Check(TokenType::IDENTIFIER)) {
        if (CheckNext(TokenType::IDENTIFIER)) {
            // <type> <var_name> = 10
            // <type> <func_name>(...) {}
            // FIXME: hardcore, dont check EOF
            TokenType type = _tokens[_curr_token_pos+2].type;

            if (type == TokenType::LPAREN) {
                return ParseFunctionDeclaration();
            }

            else if (type == TokenType::ASSIGN) {
                return ParseVariableDeclaration();
            }
        }

        // <func_name>(...)
        if (CheckNext(TokenType::LPAREN)) {
            return ParseFunctionCallStmt();
        }

        if (CheckNext(TokenType::ASSIGN)) {
            return ParseVariableAssign();
        }
    }

    if (Check(TokenType::LBRACE)) {
        return ParseBlock();
    }

    throw std::runtime_error("Unknown statement, token: " + Peek().text);
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

std::unique_ptr<IfStatementNode> Parser::ParseIfStmt()
{
    Advance(); // Skip 'if'

    Expect(TokenType::LPAREN);
    std::unique_ptr<ExpressionNode> expr = ParseExpression();
    Expect(TokenType::RPAREN);

    std::unique_ptr<BlockNode> block;

    if (Check(TokenType::LBRACE)) {
        block = ParseBlock();
    }

     

    
}

std::unique_ptr<VariableDeclNode> Parser::ParseVariableDeclaration()
{
    std::string var_type_name = Advance().text;
    std::unique_ptr<TypeNode> var_type = std::make_unique<TypeNode>(var_type_name, false); // Idk about pointer now
    std::string var_name = Advance().text;
    std::unique_ptr<ExpressionNode> expr = nullptr;
    if (Check(TokenType::ASSIGN))
    {
        Advance(); // skip =
        expr = ParseExpression();
    }
    
    Expect(TokenType::SEMICOLON);
    return std::make_unique<VariableDeclNode>(std::move(var_type), var_name, expr);

}

std::unique_ptr<FuncDeclNode> Parser::ParseFunctionDeclaration()
{
    
}

std::unique_ptr<BlockNode> Parser::ParseBlock()
{
    Advance(); // skip {

    std::vector<std::unique_ptr<StatementNode>> stmts;
    
    while (!Match(TokenType::RBRACE))
    {
        stmts.push_back(ParseStatement());
    }

    return std::make_unique<BlockNode>(std::move(stmts));
}
