#pragma once

#include "tac.h"

using AirProgram = TacProgram;

// struct AirProgram 
// {
//     TacProgram prog;
// };

class AirGenerator {
public:
    AirProgram analyze(TacProgram prog);

};