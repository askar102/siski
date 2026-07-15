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
    std::map<std::string, std::vector<Arg>> func_arg_types;
    std::map<std::string, std::string> func_ret;
    std::set<std::string> void_names;
    for (auto& fn : _prog.funcs)
    {
        func_ret[fn.name] = fn.retType;
        for (auto& param : fn.params)
        {
            func_arg_types[fn.name].push_back({param.name, param.type});
        }
    }

    for (auto& fn : _prog.funcs)
    {
        std::map<std::string, std::string> var_types;
        std::map<std::string, std::string> temp_types;
        std::map<std::string, int64_t> const_temps;

        for (auto& p : fn.params)
        {
            var_types[p.name] = p.type;
        }

        for (auto& i : fn.body)
        {
            switch(i.tag) {

                case INSTR_TAG::DECL_VAR: 
                {
                    if (i.decl_type == "U0")
                    {
                        throw std::runtime_error(std::format(
                                "AIR: DECL_VAR: Variable '{}' has incomplete type '{}'",
                                i.name, i.decl_type
                        ));
                    }

                    var_types[i.name] = i.decl_type;

                    if (i.has_init) {
                        // for const value (r-value)
                        int64_t cv;
                        if (const_val(i.lhs, const_temps, cv)) 
                        {
                            if (!can_fit(cv, i.decl_type))
                            {
                                throw std::runtime_error(std::format(
                                    "AIR: DECL_VAR: constant {} doesn't fit into '{}'", cv, i.decl_type));
                            }
                                
                            break;
                        }

                        // for non-const value (like vars)
                        std::string init_type = operand_type(i.lhs, temp_types, var_types);
                        if (init_type != i.decl_type) {
                            throw std::runtime_error(std::format(
                                "AIR: DECL_VAR: cannot init '{}' (type '{}') with value of type '{}'",
                                i.name, i.decl_type, init_type));
                        }
                    }
                    break;
                }

                case INSTR_TAG::CONST:
                {
                    i.result.data_type = "I32";
                    temp_types[i.result.name] = "I32";
                    const_temps[i.result.name] = i.lhs.constVal;
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

                    int64_t lc, rc;
                    bool l_const = const_val(i.lhs, const_temps, lc);
                    bool r_const = const_val(i.rhs, const_temps, rc);

                    if (type_op1 == type_op2)
                    {
                        i.result.data_type = type_op1;
                        temp_types[i.result.name] = type_op1;
                    }

                    else if (l_const && !r_const)
                    {
                        if (!can_fit(lc, type_op2))
                        {
                            throw std::runtime_error(std::format(
                                "AIR: BINOP: constant {} doesn't fit into '{}'", lc, type_op2));
                        }
                            
                        i.result.data_type = type_op2;
                        temp_types[i.result.name] = type_op2;
                    }

                    else if (r_const && !l_const)
                    {
                        if (!can_fit(rc, type_op1))
                        {
                            throw std::runtime_error(std::format(
                                "AIR: BINOP: constant {} doesn't fit into '{}'", rc, type_op1));
                        }
                        i.result.data_type = type_op1;
                        temp_types[i.result.name] = type_op1;
                    }

                    else 
                    {
                        throw std::runtime_error(std::format(
                                "AIR: BINOP: I cannot calculate the value ('{}' aka '{}') {} ('{}' aka '{}')",
                                lc, type_op1, i.op, rc, type_op2
                            ));
                    }
            
                    break;
                }

                case INSTR_TAG::STORE:
                {
                    if (!var_types.count(i.name))
                    {
                        throw std::runtime_error(std::format(
                                "AIR: STORE: Assignment to an undeclared variable '{}'",
                                i.name
                        ));
                    }

                    std::string var_type = var_types[i.name];

                    std::string temp_type = operand_type(i.lhs, temp_types, var_types);

                    // integral fit check
                    bool is_const = (i.lhs.type == VALUE_TYPE::CONST);
                    int64_t cval = 0;
                    if (is_const) 
                    {
                        cval = i.lhs.constVal;
                    } 
                    else if (i.lhs.type == VALUE_TYPE::TEMP && const_temps.count(i.lhs.name)) 
                    {
                        is_const = true;
                        cval = const_temps[i.lhs.name];
                    }

                    if (is_const)
                    {
                        if (!can_fit(cval, var_type))
                        {
                            throw std::runtime_error(std::format(
                            "AIR: STORE: {}(): constant {} doesn't fit into '{}'.",
                                fn.name, cval, var_type)
                            );
                        }
                        break;
                    }
                    

                    if (var_type != temp_type)
                    {
                        // type_err(var_type, temp_type);
                        throw std::runtime_error(std::format("AIR: STORE: ({} is '{}') It seems to me that type '{}' differs from '{}'", i.lhs.name, temp_type, var_type, temp_type));
                    }
                    break;
                }

                case INSTR_TAG::UNARY:
                {
                    std::string t = operand_type(i.lhs, temp_types, var_types);
                    i.result.data_type = t;
                    temp_types[i.result.name] = t;

                    int64_t cv;
                    if (const_val(i.lhs, const_temps, cv))
                    {   
                        int64_t result_val = (i.op == "-") ? -cv : cv;
                        const_temps[i.result.name] = result_val; 
                    }

                    break;
                }

                case INSTR_TAG::CALL:
                {
                    // проверка на совпадение кол-во аргументов
                    if (i.args.size() != func_arg_types[i.name].size()) {
                        throw std::runtime_error(std::format(
                            "AIR: CALL: function '{}' expects {} args, got {}",
                            i.name, func_arg_types[i.name].size(), i.args.size()
                        ));
                    }

                    for (size_t k = 0; k < i.args.size(); ++k)
                    {
                        const auto& call_arg = i.args[k];
                        const auto& src_arg = func_arg_types[i.name][k];

                        std::string arg_type = operand_type(call_arg, temp_types, var_types);

                        if (call_arg.type != VALUE_TYPE::CONST && arg_type != src_arg.type) {
                            throw std::runtime_error(std::format(
                                "AIR: CALL: Variable '{}' with type {} cannot be passed to the function 'cuz source argument '{}' has type {}.",
                                call_arg.name, arg_type, src_arg.name, src_arg.type
                            ));
                        }
                    }

                    std::string t = func_ret.count(i.name) ? func_ret[i.name] : "I32";
                    i.result.data_type = t;
                    temp_types[i.result.name] = t;
                    break;
                }

                case INSTR_TAG::RET: 
                {
                    if (fn.retType == "U0") 
                    {
                        // printf("\e[1;36mPARSE WARNING:\e[0m AIR: RET: %s(): Is function return type is %s, it's not released normally yet.\n",
                        //     fn.name.c_str(), fn.retType.c_str()
                        // );
                        break;
                    }

                    // integral fit check
                    bool is_const = (i.lhs.type == VALUE_TYPE::CONST);
                    int64_t cval = 0;
                    if (is_const) 
                    {
                        cval = i.lhs.constVal;
                    } 
                    else if (i.lhs.type == VALUE_TYPE::TEMP && const_temps.count(i.lhs.name)) 
                    {
                        is_const = true;
                        cval = const_temps[i.lhs.name];
                    }

                    if (is_const)
                    {
                        if (!can_fit(cval, fn.retType))
                        {
                            throw std::runtime_error(std::format(
                            "AIR: RET: {}(): constant {} doesn't fit into '{}'.",
                                fn.name, cval, fn.retType)
                            );
                        }
                        break;
                    }

                    std::string ret_type = operand_type(i.lhs, temp_types, var_types);

                    if (ret_type != fn.retType)
                    {
                        throw std::runtime_error(std::format(
                                "AIR: RET: {}(): You are trying to return type '{}', but your function return type is '{}'.",
                                fn.name, ret_type, fn.retType
                            ));
                    }

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
            return "I32";
        case VALUE_TYPE::TEMP:
            return temp_types.count(v.name) ? temp_types[v.name] : "I32";
        case VALUE_TYPE::VAR:
            return var_types.count(v.name) ? var_types[v.name] : "I32";
    }
    return "I32";
}