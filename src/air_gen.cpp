#include "air_gen.h"
#include "air.h"
#include "tac.h"
#include <stdexcept>

AirProgram AirGenerator::analyze()
{
    dead_code_remove();
    type_check();
    return _prog;
}

void AirGenerator::dead_code_remove()
{
    for (auto& fn : _prog.funcs) {
        std::set<std::string> used;
        for (auto& i : fn.body)
        {
            if (i.tag == INSTR_TAG::JUMP || i.tag == INSTR_TAG::IF_FALSE)
            {
                used.insert(i.name);
            }
        }

        std::vector<Instr> cleaned;
        bool reachable = true;

        for (auto& i : fn.body) 
        {
            if (i.tag == INSTR_TAG::LABEL) 
            {
                if (used.count(i.name)) 
                {
                    reachable = true;
                    cleaned.push_back(i);
                }
                
                continue;
            }

            if (!reachable)
            {
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
}

void AirGenerator::type_check()
{
    std::map<std::string, std::string> func_ret;
    for (auto& fn : _prog.funcs)
    {
        func_ret[fn.name] = fn.retType;
    }

    for (auto& fn : _prog.funcs)
    {
        std::map<std::string, std::string> var_types;
        std::map<std::string, std::string> temp_types;

        for (auto& p : fn.params)
        {
            var_types[p.name] = p.type;
        }

        for (auto& i : fn.body)
        {
            switch(i.tag) {

                case INSTR_TAG::DECL_VAR: 
                {
                    var_types[i.name] = i.decl_type;
                    break;
                }

                case INSTR_TAG::CONST:
                {
                    var_types[i.result.name] = "I32";
                    break;
                }

                case INSTR_TAG::LOAD:
                {
                    std::string t = var_types.count(i.name) ? var_types[i.name] : "I32";
                    i.result.data_type = t;
                    temp_types[i.result.name] = t;
                    break;
                }

                case INSTR_TAG::BINOP:
                {
                    std::string type_op1 = operand_type(i.lhs, temp_types, var_types);
                    std::string type_op2 = operand_type(i.rhs, temp_types, var_types);

                    if (type_op1 == type_op2)
                    {
                        i.result.data_type = type_op1;
                        temp_types[i.result.name] = type_op1;
                    }
                    else {
                        type_err(type_op1, type_op2);
                    }
            
                    break;
                }

                case INSTR_TAG::STORE:
                {
                    std::string var_type = var_types[i.name];
                    std::string temp_type = operand_type(i.lhs, temp_types, var_types);

                    if (var_type != temp_type)
                    {
                        // type_err(var_type, temp_type);
                        throw std::runtime_error(std::format("AIR: ({} is '{}') It seems to me that type '{}' differs from '{}'", i.lhs.name, temp_type, var_type, temp_type));
                    }
                    break;
                }

                case INSTR_TAG::UNARY:
                {
                    std::string t = operand_type(i.lhs, temp_types, var_types);
                    i.result.data_type = t;
                    temp_types[i.result.name] = t;
                    break;
                }

                case INSTR_TAG::CALL:
                {
                    std::string t = func_ret.count(i.name) ? func_ret[i.name] : "I32";
                    i.result.data_type = t;
                    temp_types[i.result.name] = t;
                    break;
                }

                default: break;
            }
        }
    }
}

std::string AirGenerator::operand_type( const Value& v, std::map<std::string,std::string>& temp_types, std::map<std::string,std::string>& var_types)
{
    switch (v.type) {
        case VALUE_TYPE::CONST: 
            return "i32";
        case VALUE_TYPE::TEMP:
            return temp_types.count(v.name) ? temp_types[v.name] : "I32";
        case VALUE_TYPE::VAR:
            return var_types.count(v.name) ? var_types[v.name] : "I32";
    }
    return "i32";
}