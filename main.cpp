#include "src/lexer.h"
#include "src/parser.h"
#include "src/log.h"
#include "src/pratt.h"


int main() {
    std::ifstream file("examples/numbers.ss");
    if (!file.is_open()) throw std::runtime_error("Couldn't open file");

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    LOG(" [START OF FILE DATA]\n%s\n [END OF FILE DATA]\n", fileContent.c_str());

    Lexer lex;
    lex.Tokenize(fileContent);

    // PrattParser par(lex.GetTokens());
    // size_t curr = 0;
    // par.ParseExpression(curr);

    Parser par(lex.GetTokens());
    par.ParseProgram();
}