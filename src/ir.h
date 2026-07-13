#pragma once

enum class Tag {
    CONST, // t = <number>
    BINOP, // t = a <op> b
    UNARY, // t = <op> a
    DECL, // DECL <var_name> оьявить перменную
    STORE, // STORE <var_name>, value
    LOAD, // t = load <var_name>
    LABEL, // метка
    JUMP, // бузусловный переход
    BRANCH_FALSE, // условный переход BRANCH_FALSE <cond>, <label_name>
    CALL, // t = CALL <fn_name>, [args]
    RET // RET <value>
};


class IRGen {



};