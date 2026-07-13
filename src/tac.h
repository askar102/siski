#pragma once

#include <string>
#include <cstdint>
#include <vector>

enum class INSTR_TAG {
    CONST, // t = <number>
    BINOP, // t = a <op> b
    UNARY, // t = <op> a
    DECL_VAR, // DECL <var_name> оьявить перменную
    STORE, // STORE <var_name>, value
    LOAD, // t = load <var_name>
    LABEL, // метка
    JUMP, // бузусловный переход
    IF_FALSE, // условный переход IF_FALSE <cond>, <label_name>
    CALL, // t = CALL <fn_name>, [args]
    RET // RET <value>
};

enum class VALUE_TYPE {
    CONST,
    TEMP,
    VAR
};

struct Value {
    VALUE_TYPE type;
    std::string name;
    int32_t constVal = 0;

    std::string data_type = ""; // idk

    static Value Temp(std::string n) { return {VALUE_TYPE::TEMP, std::move(n), 0}; }
    static Value Var(std::string n)  { return {VALUE_TYPE::VAR,  std::move(n), 0}; }
    static Value Const(int32_t v)    { return {VALUE_TYPE::CONST, "", v}; }
};

struct Instr {
    INSTR_TAG tag;  
    Value result; 
    Value lhs; // first operand
    Value rhs; // second operand
    std::string op; 
    std::string name; 
    std::vector<Value> args;

    std::string decl_type;
};

struct Param {
    std::string type;
    std::string name;
    bool hasDefault;
};

struct TacFunc {    
    std::string name;
    std::vector<Param> params;
    std::string retType;
    std::vector<Instr> body;
};

struct TacProgram {
    std::vector<TacFunc> funcs;
};
