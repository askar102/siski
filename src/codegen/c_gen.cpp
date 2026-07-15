#include "c_gen.h"
#include <cstdio>
#include <ios>
#include <string>

std::string CGen::val_to_c(const Value& v)
{
    switch (v.type) {
        case VALUE_TYPE::CONST: 
        {
            return std::to_string(v.constVal);
        }
        case VALUE_TYPE::TEMP:
        case VALUE_TYPE::VAR:
            return v.name;
    }
    return "";
}

void CGen::generate(const AirProgram& prog)
{
    file = fopen("cgen.c", "w+");

    puts_ins(file, "#include <stdint.h>\n\n");

    for (auto& fn : prog.get_funcs()) {
        // puts_ins(file, "{} {}(", fn.retType, fn.name);
        // for (size_t k = 0; k < fn.params.size(); ++k)
        // {
        //     puts_ins(file, "{}{} {}", k ? ", " : "", fn.params[k].type, fn.params[k].name);
        // }
        // puts_ins(file, ");\n");

        puts_func(file, fn, ";");
    }
    puts_ins(file, "\n");


    for (auto& fn : prog.get_funcs()) {
        // std::set<std::string> unique_temps;
        int32_t temp_counter = 0;

        // fprintf(file, "%s %s(", fn.retType.c_str(), fn.name.c_str());
        // puts_ins(file, "");
        // for (size_t k = 0; k < fn.params.size(); ++k)
        // {
        //     fprintf(file, "%s%s %s", k ? ", " : "", fn.params[k].type.c_str(), fn.params[k].name.c_str());
        // } 
        // fprintf(file, ") {\n");
        puts_func(file, fn, " {");


        std::map<std::string, std::string> temp_types;
        for (auto& i : fn.body)
        {
            if (!i.result.name.empty() && i.result.type == VALUE_TYPE::TEMP)
            {
                temp_types[i.result.name] = i.result.data_type.empty() ? "I32" : i.result.data_type;
            }
        }

        for (auto& [name, type] : temp_types)
        {
            puts_ins(file, "\t{} {};\n", to_c_type(type), name);
        }

        for (auto& i : fn.body) {
            if (i.tag == INSTR_TAG::DECL_VAR) {
                puts_ins(file, "\t{} {};\n", to_c_type(i.decl_type), i.name);
            }
        }

        // for (const auto& temp_name : unique_temps) {
        //     puts_ins(file, "\tint {};\n", temp_name);
        // }

        // for (auto& i : fn.body) {
        //     if (i.tag == INSTR_TAG::DECL_VAR) {
        //         fprintf(file, "    %s %s = %s;\n", 
        //             i.decl_type.c_str(), i.name.c_str(), val_to_c(i.lhs).c_str());
        //         continue;
        //     }
        // }

        for (auto& i : fn.body) {
            switch (i.tag) {
                case INSTR_TAG::CONST: 
                {
                    // std::println(file, " {} = {};", i.result.name, i.lhs.constVal);
                    puts_ins(file, "\t{} = {};\n", i.result.name, i.lhs.constVal);
                    break;
                }
                    
                case INSTR_TAG::BINOP: 
                {
                    puts_ins(file, "\t{} = {} {} {};\n", i.result.name, val_to_c(i.lhs), i.op, val_to_c(i.rhs));
                    break;
                }
                    
                case INSTR_TAG::UNARY: 
                {
                    puts_ins(file, "\t{} = {}{};\n", i.result.name, i.op, val_to_c(i.lhs));
                    break;
                }

                case INSTR_TAG::LOAD: 
                {
                    puts_ins(file, "\t{} = {};\n", i.result.name, i.name);
                    break;
                }

                case INSTR_TAG::STORE: 
                {
                    puts_ins(file, "\t{} = {};\n", i.name, val_to_c(i.lhs));
                    break;
                }

                case INSTR_TAG::DECL_VAR: 
                {
                    puts_ins(file, "\t{} = {};\n", i.name, val_to_c(i.lhs));
                    break;
                }
                
                case INSTR_TAG::CALL: 
                {
                    // void check
                    if (i.result.data_type == "U0") 
                    {
                        puts_ins(file, "\t{}(", i.name);
                    }
                    else
                    {
                        puts_ins(file, "\t{} = {}(", i.result.name, i.name);
                    }
                    
                    for (size_t k = 0; k < i.args.size(); ++k)
                    {
                        puts_ins(file, "\t{}{}", k ? ", " : "", val_to_c(i.args[k]));
                    }

                    puts_ins(file, ");\n");
                    break;
                }

                case INSTR_TAG::RET:
                {
                    puts_ins(file, "\treturn {};\n", val_to_c(i.lhs));
                    break;
                }
                    
                case INSTR_TAG::LABEL:
                {
                    puts_ins(file, "{}:\n", i.name);
                    break;
                }
                    
                case INSTR_TAG::JUMP:
                {
                    puts_ins(file, "\tgoto {};\n", i.name);
                    break;
                }
                    
                case INSTR_TAG::IF_FALSE:
                {
                    puts_ins(file, "\tif (!{}) goto {};\n", val_to_c(i.lhs), i.name);
                    break;
                }
            }
        }
        puts_ins(file, "{}\n\n", "}");
    }

    fclose(file);
}