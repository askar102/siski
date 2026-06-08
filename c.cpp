#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <utility>

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
                continue;
            }


            // check exp. operators ->  keywords 
            if (std::isalpha(content[i])) {
                printf("pozda%c\n", content[i]);
                CheckAlpha(content, i);
                continue;
            }
            

            CheckAloneOperators(content, i);
            continue;
        }   
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
            // FIXME: kostil
            --i;

            _tokens.push_back({TokenType::INT_LITERAL, value});

            LOG("DIGIT: %s\n", value.c_str());
        }
    }

    void CheckAlpha(const std::string& content, size_t& i) {
        if (std::isalpha(content[i])) {
            std::string value;

            while (i < content.size() && (std::isalnum(content[i])) || content[i] == '_') {
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
                LOG("LBRACE: }\n");
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
        // if (!keyword.empty()) {
        //     if (keyword == "if") {
        //         return;
        //     }
        // }
        return;
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
}