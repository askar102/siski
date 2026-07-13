#include "src/lexer.h"
#include "src/parser.h"
#include "src/log.h"
#include "src/printAst.h"


int main() {
    std::ifstream file("examples/numbers.ss");
    if (!file.is_open()) throw std::runtime_error("Couldn't open file");

    std::string fileContent((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    LOG(" [START OF FILE DATA]\n%s\n [END OF FILE DATA]\n", fileContent.c_str());

    Lexer lex;
    lex.Tokenize(fileContent);

    try {
        Parser par(lex.GetTokens());
        auto ast = par.ParseProgram();
        
        PrintAst printer;
        ast->accept(printer);

    } catch (const std::exception& e) {
        printf("PARSE ERROR: %s\n", e.what());
    }
}