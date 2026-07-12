#include "lexer.h"

void Lexer::Tokenize(const std::string& content) {
    for (size_t i = 0; i < content.size(); ++i) {
        // comment checker
        if (content[i] == '/' && content[i+1] == '*') {
            i += 2;
            while (i < content.size() && !(content[i] == '*' && content[i+1] == '/')) {
                i++;
            }
            i += 2;
            continue;
        }

        if (std::isspace(content[i])) {
            continue;
        }

        else if (std::isdigit(content[i])) {
            CheckDigit(content, i);
            --i;
            continue;
        }


        // check exp. operators ->  keywords 
        else if (std::isalpha(content[i])) {
            CheckAlpha(content, i);
            --i;
            continue;
        }
        

        CheckAloneOperators(content, i);
        continue;
    }

    _tokens.push_back({TokenType::C_EOF, ""});
    LOG("EOF REACHED\n");
}

const std::vector<Token>& Lexer::GetTokens() const {
    return _tokens;
}

void Lexer::CheckDigit(const std::string& content, size_t& i) {
    if (std::isdigit(content[i])) {
        std::string value;

        while (i < content.size() && (std::isdigit(content[i]))) {
            value += content[i];
            ++i;
        }

        _tokens.push_back({TokenType::NUMBER, value});

        LOG("DIGIT: %s\n", value.c_str());
    }
}

void Lexer::CheckAlpha(const std::string& content, size_t& i) {
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

void Lexer::CheckAloneOperators(const std::string& content, size_t& i) {
    switch (content[i]) {
        case '+':  {
            _tokens.push_back({TokenType::PLUS, "+"});
            LOG("PLUS: +\n");
            break;
        }

        case '-': {
            if (i < content.size() && content[i+1] == '>') {
                _tokens.push_back({TokenType::ARROW_OP, "->"});
                LOG("FUNC_RETURN_TYPE_OP: ->\n");
                ++i;
                break;
            }

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

        case ':': {
            _tokens.push_back({TokenType::COLON, ":"});
            LOG("COLON: :\n");
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
                _tokens.push_back({TokenType::LESS_EQUAL, "<="});
                LOG("LESS_EQUAL: <=\n");
                ++i;
                break;
            }

            _tokens.push_back({TokenType::LESS, "<"});
            LOG("LESS: <\n");
            break;
        }

        case '>': {
            if (i < content.size() && content[i+1] == '=') {
                _tokens.push_back({TokenType::GREATER_EQUAL, ">="});
                LOG("GREATER_EQUAL: >=\n");
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

void Lexer::CheckKeywords(const std::string& keyword) {
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

        else if (keyword == "return") {
            _tokens.push_back({TokenType::RETURN, "return"});
            LOG("RETURN: return\n");
            return;
        }

        else if (keyword == "exC") {
            _tokens.push_back({TokenType::EXTERN_C, "exC"});
            LOG("EXTERN_C: exC\n");
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