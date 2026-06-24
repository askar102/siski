#include <cctype>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <utility>

#include "c.h"


// SISKI COMPILER

inline void LOG(const std::string& str) {
    std::printf("%s", str.c_str());
}

template <typename... Args>
inline void LOG(const char* str, Args... args) {
    std::printf(str, args...);
}

enum TokenType {
    // keywords
    IF, ELSE, 
    GOTO, LABEL,
    INT, VOID,
    FUNC, RETURN,

    // another
    IDENTIFIER, INT_LITERAL,

    // expression operators
    ASSIGN, PLUS, MINUS, STAR, SLASH, AMPERSAND,
    // condition operators
    EQUAL, LESS, GREATER, EQUAL_AND_LESS, EQUAL_AND_GREATER,


    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, SEMICOLON, COMMA,

    C_EOF, C_ERROR,
};


class Lexer {
public:
    Lexer() = default ;

    void Tokenize(const std::string& content) {
        for (size_t i = 0; i <= content.size()-1; ++i) {
            if (std::isspace(content[i])) {
                continue;
            }

            if (std::isdigit(content[i])) {
                CheckDigit(content, i);
            }


            // check exp. operators ->  keywords 
            if (std::isalpha(content[i])) {
                CheckAlpha(content, i);
            }
            

            CheckAloneOperators(content, i);
            continue;
        }

        _tokens.push_back({TokenType::C_EOF, ""});
        LOG("EOF REACHED\n");
    }  

    const std::vector<std::pair<TokenType, std::string>>& GetTokens() const {
        return _tokens;
    }

private:
    std::vector<std::pair<TokenType, std::string>> _tokens;

    void CheckDigit(const std::string& content, size_t& i) {
        if (std::isdigit(content[i])) {
            std::string value;

            while (i < content.size() && (std::isdigit(content[i]))) {
                value += content[i];
                ++i;
            }

            _tokens.push_back({TokenType::INT_LITERAL, value});

            LOG("DIGIT: %s\n", value.c_str());
        }
    }

    void CheckAlpha(const std::string& content, size_t& i) {
        if (std::isalpha(content[i])) {
            std::string value;

            while (i < content.size() && (std::isalnum(content[i]) || content[i] == '_')) {
                value += content[i];
                ++i;
            }

            LOG("FOUND WORD: %s\n", value.c_str());
            
            CheckKeywords(value);
        }
    }

    void CheckAloneOperators(const std::string& content, size_t& i) {
        switch (content[i]) {
            case '+':  {
                _tokens.push_back({TokenType::PLUS, "+"});
                LOG("PLUS: +\n");
                break;
            }

            case '-': {
                _tokens.push_back({TokenType::MINUS, "-"});
                LOG("MINUS: -\n");
                break;
            }

            case '*': {
                _tokens.push_back({TokenType::STAR, "*"});
                LOG("STAR: *\n");
                break;
            }

            case '/': {
                _tokens.push_back({TokenType::SLASH, "/"});
                LOG("SLASH: /\n");
                break;
            }

            case '(': {
                _tokens.push_back({TokenType::LPAREN, "("});
                LOG("LPAREN: (\n");
                break;
            }

            case ')': {
                _tokens.push_back({TokenType::RPAREN, ")"});
                LOG("RPAREN: )\n");
                break;
            }

            case '{': {
                _tokens.push_back({TokenType::LBRACE, "{"});
                LOG("LBRACE: {\n");
                break;
            }

            case '}': {
                _tokens.push_back({TokenType::RBRACE, "}"});
                LOG("RBRACE: }\n");
                break;
            }

            case '[': {
                _tokens.push_back({TokenType::LBRACKET, "["});
                LOG("LBRACKET: [\n");
                break;
            }

            case ']': {
                _tokens.push_back({TokenType::RBRACKET, "]"});
                LOG("RBRACKET: ]\n");
                break;
            }

            case ';': {
                _tokens.push_back({TokenType::SEMICOLON, ";"});
                LOG("SEMICOLON: ;\n");
                break;
            }

            case ',': {
                _tokens.push_back({TokenType::COMMA, ","});
                LOG("COMMA: ,\n");
                break;
            }

            case '&': {
                _tokens.push_back({TokenType::AMPERSAND, "&"});
                LOG("AMPERSAND: &\n");
                break;
            }

            case '=': {
                if (i < content.size() && content[i+1] == '=') {
                    _tokens.push_back({TokenType::EQUAL, "=="});
                    LOG("EQUAL: ==\n");
                    ++i;
                    break;
                }

                _tokens.push_back({TokenType::ASSIGN, "="});
                LOG("ASSIGN: =\n");
                break;
            }

            case '<': {
                // todo =<
                if (i < content.size() && content[i+1] == '=') {
                    _tokens.push_back({TokenType::EQUAL_AND_LESS, "<="});
                    LOG("QUAL_AND_LESS: <=\n");
                    ++i;
                    break;
                }

                _tokens.push_back({TokenType::LESS, "<"});
                LOG("LESS: <\n");
                break;
            }

            case '>': {
                if (i < content.size() && content[i+1] == '=') {
                    _tokens.push_back({TokenType::EQUAL_AND_GREATER, ">="});
                    LOG("QUAL_AND_GREATER: >=\n");
                    ++i;
                    break;
                }

                _tokens.push_back({TokenType::GREATER, ">"});
                LOG("GREATER: >\n");
                break;
            }

            default: {
                LOG("CANT FIND OPERATOR SUKA: %c\n", content[i]);
                break;
            }
        }
    }

    void CheckKeywords(const std::string& keyword) {
        if (!keyword.empty()) {
            if (keyword == "if") {
                _tokens.push_back({TokenType::IF, "if"});
                LOG("IF: if\n");
                return;
            }
            else if (keyword == "else") {
                _tokens.push_back({TokenType::ELSE, "else"});
                LOG("ELSE: else\n");
                return;
            }
            else if (keyword == "goto") {
                _tokens.push_back({TokenType::GOTO, "goto"});
                LOG("GOTO: goto\n");
                return;
            }
            else if (keyword == "label") {
                _tokens.push_back({TokenType::LABEL, "label"});
                LOG("LABEL: label\n");
                return;
            }

            else if (keyword == "int") {
                _tokens.push_back({TokenType::INT, "int"});
                LOG("INT: int\n");
                return;
            }
            else if (keyword == "void") {
                _tokens.push_back({TokenType::VOID, "void"});
                LOG("VOID: void\n");
                return;
            }

            else if (keyword == "func") {
                _tokens.push_back({TokenType::FUNC, "func"});
                LOG("FUNC: func\n");
                return;
            }

            else if (keyword == "return") {
                _tokens.push_back({TokenType::RETURN, "return"});
                LOG("RETURN: return\n");
                return;
            }

            // IDENT CHECK
            else {
                _tokens.push_back({TokenType::IDENTIFIER, keyword});
                LOG("INDENT: %s\n", keyword.c_str());
                return;
            }
        }

        return;
    }
};

class Parser {
public:
    Parser(const std::vector<std::pair<TokenType, std::string>>& tokens) : _tokens(tokens), _currentToken(0) {}

    std::vector<std::unique_ptr<FunctionAST>> ParseProgram() {
        std::vector<std::unique_ptr<FunctionAST>> functions;

        while(Current().first != TokenType::C_EOF) {
            if (Current().first == TokenType::FUNC) {
                functions.push_back(ParseFunction());
            }

            else {
                fprintf(stderr, "PLS WRITE ONLY FUNCTION IN GLOBAL BLOCK!\n");
                exit(0);
            }
        }

        return functions;
    }

private:
    std::unique_ptr<FunctionAST> ParseFunction() {
        Except(TokenType::FUNC);

        // proto - name, args
        auto proto = ParsePrototype();

        // body - Expr list
        auto body = ParseBlock();

        return std::make_unique<FunctionAST>(std::move(proto), std::move(body));
    }


    std::unique_ptr<PrototypeAST> ParsePrototype() {
        auto ident = Except(TokenType::IDENTIFIER);
        std::string funcName = ident.second;

        Except(TokenType::LPAREN);

        std::vector<std::string> args;
        while (Current().first != TokenType::RPAREN) {
            Except(TokenType::INT);

            std::string argName = Except(TokenType::IDENTIFIER).second;
            args.push_back(argName);

            if (Current().first == TokenType::COMMA) {
                Except(TokenType::COMMA);
            }
        }

        Except(TokenType::RPAREN);  

        return std::make_unique<PrototypeAST>(funcName, std::move(args));   
    }

    std::unique_ptr<ExprAST> ParseBlock() {
        Except(TokenType::LBRACE);

        std::unique_ptr<ExprAST> lastExpr = nullptr;

        while (Current().first != TokenType::RBRACE) {
            lastExpr = ParseExpression();

            Except(TokenType::SEMICOLON);
        }

        Except(TokenType::RBRACE);
        return lastExpr;
    }   

    std::unique_ptr<ExprAST> ParseExpression() {
        // number flow
        if (Current().first == TokenType::INT_LITERAL) {
            auto token = Except(TokenType::INT_LITERAL);
            return std::make_unique<NumberExprAST>(std::stoul(token.second));
        }

        if (Current().first == TokenType::IDENTIFIER) {
            auto token = Except(TokenType::IDENTIFIER);

            if (Current().first == TokenType::LPAREN) {
                Except(TokenType::LPAREN);
                std::vector<std::unique_ptr<ExprAST>> callArgs;

                while (Current().first != TokenType::RPAREN) {
                    callArgs.push_back(ParseExpression());
                    if (Current().first == TokenType::COMMA) Except(TokenType::COMMA);
                }

                Except(TokenType::RPAREN);

                return std::make_unique<CallExprAST>(token.second, std::move(callArgs));

            }

            return std::make_unique<VariableExprAST>(token.second);

        }

        std::printf("Unknown token in expression: %s\n", Current().second.c_str());
        exit(1);
    }
    

    size_t _currentToken;
    const std::vector<std::pair<TokenType, std::string>>& _tokens;

    const std::pair<TokenType, std::string>& Current() {
        return _tokens[_currentToken];
    }

    const std::pair<TokenType, std::string>& Except(TokenType type) {
        if (Current().first != type) {
            fprintf(stderr, "PIZDA: SYNTAX ERROR AT TOKEN NUMBER %d\n", _currentToken);
            exit(1);
        }

        return _tokens[_currentToken++];
    }

    const std::pair<TokenType, std::string>& Peek(size_t offset = 1) {
        if (_currentToken + offset >= _tokens.size()) {
            return _tokens.back();
        }

        return _tokens[_currentToken + offset];
    }

};

int main() {
    std::ifstream file("file.ss");
    if (!file.is_open()) return 1;

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    printf(" [START OF FILE DATA]\n%s\n [END OF FILE DATA]\n", fileContent.c_str());

    Lexer lex;
    lex.Tokenize(fileContent);

    Parser par(lex.GetTokens());
    par.ParseProgram();
}