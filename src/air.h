#pragma once

#include "tac.h"


class AirProgram {
private:
    TacProgram _prog;
public:
    AirProgram(TacProgram prog)
        : _prog(std::move(prog)) {}

    const std::vector<TacFunc>& get_funcs() const { return _prog.funcs; }
    std::vector<TacFunc>& get_funcs() { return _prog.funcs; }

    const TacProgram& get_prog() const { return _prog; }
};

