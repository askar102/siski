#include "air_gen.h"
#include "tac.h"

AirProgram AirGenerator::analyze(TacProgram prog)
{
    for (auto& fn : prog.funcs) {
        std::vector<Instr> cleaned;
        bool reachable = true;

        // 1. dead code after JUMP/RET ( то ест убираем те до которых управление не доходит )
        for (auto& i : fn.body) {
            if (i.tag == INSTR_TAG::LABEL) {
                reachable = true;
                cleaned.push_back(i);
                continue; 
            }

            if (!reachable) {
                continue;
            }

            cleaned.push_back(i);

            if (i.tag == INSTR_TAG::JUMP || i.tag == INSTR_TAG::RET)
            {
                reachable = false;
            }
        }

        fn.body = std::move(cleaned);
    } 

    return prog;
}