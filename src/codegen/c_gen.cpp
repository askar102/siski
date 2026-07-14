#include "c_gen.h"
#include <string>

    std::string CGen::val_to_c(const Value& v)
    {
        switch (v.type) {
            case VALUE_TYPE::CONST: 
                return std::to_string(v.constVal);
            case VALUE_TYPE::TEMP:
            case VALUE_TYPE::VAR:
                return v.name;
        }
        return "???";
    }

    void CGen::generate(const AirProgram& prog)
    {
        file = fopen("cgen.c", "w+");

        std::set<std::string> unique_temps;
        
        for (auto& fn : prog.funcs) {
            fprintf(file, "%s %s(", fn.retType.c_str(), fn.name.c_str());
            for (size_t k = 0; k < fn.params.size(); ++k)
            {
                fprintf(file, "%s%s %s", k ? ", " : "", fn.params[k].type.c_str(), fn.params[k].name.c_str());
            }
            fprintf(file, ");\n");
        }
        fprintf(file, "\n");


        for (auto& fn : prog.funcs) {
            std::vector<Instr> func_var_decls;
            int32_t temp_counter = 0;

            fprintf(file, "%s %s(", fn.retType.c_str(), fn.name.c_str());
            for (size_t k = 0; k < fn.params.size(); ++k)
            {
                fprintf(file, "%s%s %s", k ? ", " : "", fn.params[k].type.c_str(), fn.params[k].name.c_str());
            } 
            fprintf(file, ") {\n");

            for (auto& i : fn.body) {
                if (i.tag == INSTR_TAG::DECL_VAR) {
                    fprintf(file, "    %s %s = %s;\n", 
                        i.decl_type.c_str(), i.name.c_str(), val_to_c(i.lhs).c_str());
                    continue;
                }
            }

            for (auto& i : fn.body) {
                if (i.tag == INSTR_TAG::CONST  || i.tag == INSTR_TAG::BINOP || 
                    i.tag == INSTR_TAG::UNARY  || i.tag == INSTR_TAG::LOAD  || 
                    i.tag == INSTR_TAG::CALL) 
                {
                    if (!i.result.name.empty() && i.result.name[0] == 't') {
                        unique_temps.insert(i.result.name);
                    }
                }
            }

            for (const auto& temp_name : unique_temps) {
                fprintf(file, "    int %s;\n", temp_name.c_str());
            }

            for (auto& i : fn.body) {
                switch (i.tag) {
                    case INSTR_TAG::CONST: {
                        fprintf(file, "    %s = %d;\n", i.result.name.c_str(), i.lhs.constVal);
                        break;
                    }
                        
                    case INSTR_TAG::BINOP:
                        fprintf(file, "    %s = %s %s %s;\n", i.result.name.c_str(),
                            val_to_c(i.lhs).c_str(), i.op.c_str(), val_to_c(i.rhs).c_str());
                        break;
                    case INSTR_TAG::UNARY:
                        fprintf(file, "    %s = %s%s;\n", i.result.name.c_str(),
                            i.op.c_str(), val_to_c(i.lhs).c_str());
                        break;
                    case INSTR_TAG::LOAD:
                        fprintf(file, "    %s = %s;\n", i.result.name.c_str(), i.name.c_str());
                        break;
                    case INSTR_TAG::STORE:
                        fprintf(file, "    %s = %s;\n", i.name.c_str(), val_to_c(i.lhs).c_str());
                        break;
                    // case INSTR_TAG::DECL_VAR:
                    //     fprintf(file, "    %s %s = %s;\n", i.decl_type.c_str(),
                    //         i.name.c_str(), val_to_c(i.lhs).c_str());
                    //     break;
                    case INSTR_TAG::CALL: {
                        fprintf(file, "    %s = %s(", i.result.name.c_str(), i.name.c_str());
                        for (size_t k = 0; k < i.args.size(); ++k)
                            fprintf(file, "%s%s", k ? ", " : "", val_to_c(i.args[k]).c_str());
                        fprintf(file, ");\n");
                        break;
                    }
                    case INSTR_TAG::RET:
                        fprintf(file, "    return %s;\n", val_to_c(i.lhs).c_str());
                        break;
                    case INSTR_TAG::LABEL:
                        fprintf(file, "%s:\n", i.name.c_str());
                        break;
                    case INSTR_TAG::JUMP:
                        fprintf(file, "    goto %s;\n", i.name.c_str());
                        break;
                    case INSTR_TAG::IF_FALSE:
                        fprintf(file, "    if (!%s) goto %s;\n", val_to_c(i.lhs).c_str(), i.name.c_str());
                        break;
                }
            }
            fprintf(file, "}\n\n");
        }

        fclose(file);
    }