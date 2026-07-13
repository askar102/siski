#include "src/lexer.h"
#include "src/parser.h"
#include "src/log.h"
// #include "src/print_ast.h"
#include "src/tac_gen.h"

#include "src/air.h"
#include "src/codegen/c_gen.h"


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
        
        // PrintAst printer;
        // ast->accept(printer);
        TacGenVisitor tac_gen;
        TacProgram tac = tac_gen.gen(*ast);
        TacGenVisitor::dump_tac(tac);

        AirGenerator air_gen;
        AirProgram air_out = air_gen.analyze(tac);

        CGen gen;
        gen.generate(air_out);

    } catch (const std::exception& e) {
        printf("PARSE ERROR: %s\n", e.what());
    }
}