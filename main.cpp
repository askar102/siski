#include "src/lexer.h"
#include "src/log.h"


int main() {
    std::ifstream file("examples/main.ss");
    if (!file.is_open()) return 1;

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    LOG(" [START OF FILE DATA]\n%s\n [END OF FILE DATA]\n", fileContent.c_str());

    Lexer lex;
    lex.Tokenize(fileContent);

//     Parser par(lex.GetTokens());
//     par.ParseProgram();
}