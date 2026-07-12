#include "parser.h"
#include "pratt.h"

Parser::Parser(const std::vector<Token>& tokens)
    : _tokens(tokens), pratt(std::make_unique<PrattParser>(tokens)) {}

Parser::~Parser() = default;

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
            return ParseVariableDeclaration();
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

    Expect(TokenType::SEMICOLON);
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
    Advance(); // skip 'if'

    Expect(TokenType::LPAREN);
    std::unique_ptr<ExpressionNode> cond = ParseExpression();
    Expect(TokenType::RPAREN);

    std::unique_ptr<StatementNode> then_block = ParseStatement();

    std::unique_ptr<StatementNode> else_block = nullptr;
    if (Match(TokenType::ELSE))
    {
        else_block = ParseStatement();
    }

    return std::make_unique<IfStatementNode>(std::move(cond), std::move(then_block), std::move(else_block));
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
    return std::make_unique<VariableDeclNode>(std::move(var_type), var_name, std::move(expr));

}

std::unique_ptr<FuncDeclNode> Parser::ParseFunctionDeclaration()
{
    // <type> <name>(...) {...}

    std::string func_type_name = Advance().text;
    std::unique_ptr<TypeNode> func_type = std::make_unique<TypeNode>(func_type_name, false); 

    std::string func_name = Advance().text;
    std::vector<std::unique_ptr<ArgNode>> func_args;

    Expect(TokenType::LPAREN);

    if (!Check(TokenType::RPAREN))
    {
        func_args.push_back(ParseArg());
        while (Match(TokenType::COMMA))
        {
            func_args.push_back(ParseArg());
        }  
    }

    Advance(); // skip )

    std::unique_ptr<BlockNode> func_block = nullptr;

    if (Check(TokenType::LBRACE)) 
    {
        func_block = ParseBlock();
    }
    else 
    {
        Expect(TokenType::SEMICOLON);
    }

    return std::make_unique<FuncDeclNode>(func_name, std::move(func_args), std::move(func_type), std::move(func_block));
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

std::unique_ptr<ArgNode> Parser::ParseArg()
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
    
    return std::make_unique<ArgNode>(var_name, std::move(var_type), std::move(expr));
}

std::unique_ptr<RootNode> Parser::ParseProgram()
{
    std::vector<std::unique_ptr<FuncDeclNode>> funcs;

    while (!Check(TokenType::C_EOF)) {
        funcs.push_back(ParseFunctionDeclaration());
    }

    return std::make_unique<RootNode>(std::move(funcs));
}