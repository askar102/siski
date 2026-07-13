#include "parser.h"
#include "ast.h"
#include <memory>

Parser::Parser(const std::vector<Token>& tokens)
    : _tokens(tokens) {}

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

int32_t Parser::lbp(TokenType type) const
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
        case TokenType::LPAREN: return 30;
        default: return 0;
    }
}

std::string Parser::opChar(TokenType t) const {
    switch (t) {
        case TokenType::PLUS:  return "+";
        case TokenType::MINUS: return "-";
        case TokenType::STAR:  return "*";
        case TokenType::SLASH: return "/";
        case TokenType::LESS: return "<";
        case TokenType::GREATER: return ">";
        case TokenType::GREATER_EQUAL: return ">=";
        case TokenType::LESS_EQUAL: return "<=";
        case TokenType::EQUAL: return "==";
        default: return "?";
    }
}

std::unique_ptr<ExpressionNode> Parser::nud()
{
    Token t = Peek();
    switch (t.type) {
        case TokenType::NUMBER: {        
            Advance();
            return std::make_unique<NumberNode>(std::stoi(t.text));
        }

        case TokenType::MINUS: {
            Advance();
            return std::make_unique<UnaryNode>("-", parsePratt(100));
        }

        case TokenType::IDENTIFIER: {
            Advance();
            return std::make_unique<VariableRefNode>(t.text);
        }

        case TokenType::LPAREN: {
            Advance();

            std::unique_ptr<ExpressionNode> inner = parsePratt(0);

            if (Peek().type != TokenType::RPAREN) 
                throw std::runtime_error("Expected ')'");

            Advance();
            return inner;
        }

        default:
            throw std::runtime_error("Unexpected token in nud");
    }
}

std::unique_ptr<ExpressionNode> Parser::led(std::unique_ptr<ExpressionNode> left)
{
    Token t = Peek();

    if (Check(TokenType::LPAREN))
    {
        Advance();
        std::vector<std::unique_ptr<ExpressionNode>> args;
        if (!Check(TokenType::RPAREN)) {
            args.push_back(parsePratt(0));
            while (Match(TokenType::COMMA))
                args.push_back(parsePratt(0));
        }
        Expect(TokenType::RPAREN);

        auto* ref = dynamic_cast<VariableRefNode*>(left.get());
        if (!ref)
        {
            throw std::runtime_error("LED: Call target is not a name.");
        }
        std::string fname = ref->get_name();
        return std::make_unique<FunctionCallNode>(fname, std::move(args));
    }

    int32_t bp = lbp(t.type);
    std::string op = opChar(t.type);
    Advance(); 
    std::unique_ptr<ExpressionNode> right = parsePratt(bp);
    return std::make_unique<BinaryExpression>(std::move(left), op, std::move(right));
}

std::unique_ptr<ExpressionNode> Parser::parsePratt(int32_t rbp) {
    std::unique_ptr<ExpressionNode> left = nud();
    while (rbp < lbp(Peek().type)) {
        left = led(std::move(left));
    }
    return left;
}

// END OF PRATT

std::unique_ptr<ExpressionNode> Parser::ParseExpression()
{
    return parsePratt(0);
}

std::unique_ptr<StatementNode> Parser::ParseStatement()
{
    // LOG("ParseStatement at pos=%zu token=%s\n", _curr_token_pos, Peek().text.c_str());

    if (Check(TokenType::IF)) {
        return ParseIfStmt();
    }

    if (Check(TokenType::GOTO)) {
        return ParseGotoStmt();
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
            auto exp = ParseExpression();
            Expect(SEMICOLON);
            return std::make_unique<ExpressionStatement>(std::move(exp));
        }

        if (CheckNext(TokenType::ASSIGN)) {
            return ParseVariableAssign();
        }

        if (CheckNext(TokenType::COLON)) {
            return ParseLabelStmt();
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
    Expect(TokenType::SEMICOLON); 
    return std::make_unique<GotoStatement>(t.text);
}

std::unique_ptr<LabelStatement> Parser::ParseLabelStmt()
{
    std::string label_name = Advance().text;

    Expect(TokenType::COLON);

    return std::make_unique<LabelStatement>(label_name);
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

// std::unique_ptr<FunctionCallNode> Parser::ParseFunctionCallStmt()
// {
//     std::string func_name = Advance().text;
//     std::vector<std::unique_ptr<ExpressionNode>> call_args;

//     Expect(TokenType::LPAREN);

//     if (!Check(TokenType::RPAREN))
//     {
//         call_args.push_back(ParseExpression());
//         while (Match(TokenType::COMMA))
//         {
//             call_args.push_back(ParseExpression());
//         }  
//     }

//     Expect(TokenType::RPAREN);
//     Expect(TokenType::SEMICOLON);

//     return std::make_unique<FunctionCallNode>(func_name, std::move(call_args));
// }

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