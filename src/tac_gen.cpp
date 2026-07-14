#include "tac_gen.h"
#include "ast.h"
#include "tac.h"
#include <stdexcept>
#include <format>

void TacGenVisitor::dump_tac(const TacProgram& prog)
{
    for (auto& fn : prog.funcs)
    {
        printf("FUNC %s(ret_type: %s):\n", fn.name.c_str(), fn.retType.c_str());
        for (auto& i : fn.body)
        {
            switch (i.tag) {
                case INSTR_TAG::CONST: {
                    printf("%s = CONST %d\n", i.result.name.c_str(), i.lhs.constVal);
                    break;
                }

                case INSTR_TAG::BINOP: {
                    printf("%s = %s %s %s\n", 
                        i.result.name.c_str(), val_to_str(i.lhs).c_str(), i.op.c_str(), val_to_str(i.rhs).c_str()
                    );
                    break;
                }

                case INSTR_TAG::RET: {
                    printf("RET %s\n", val_to_str(i.lhs).c_str());
                    break;
                }

                case INSTR_TAG::LOAD: {
                    printf("%s = LOAD %s\n", i.result.name.c_str(), i.name.c_str());
                    break;
                }

                case INSTR_TAG::DECL_VAR: {
                    printf("DECL_VAR %s (type: %s, value: %s)\n",
                        i.name.c_str(), i.decl_type.c_str(), val_to_str(i.lhs).c_str()
                    );
                    break;
                }

                case INSTR_TAG::STORE: {
                    printf("STORE %s, %s\n",
                        i.name.c_str(), val_to_str(i.lhs).c_str()
                    );
                    break;
                }

                case INSTR_TAG::IF_FALSE: {
                    printf("IF_FALSE %s, %s\n",
                        val_to_str(i.lhs).c_str(), i.name.c_str()
                    );
                    break;
                }

                case INSTR_TAG::LABEL: {
                    printf("LABEL %s:\n",
                        i.name.c_str()
                    );
                    break;
                }

                case INSTR_TAG::JUMP: {
                    printf("JUMP %s\n",
                        i.name.c_str()
                    );
                    break;
                }

                case INSTR_TAG::CALL: {
                    printf("%s = CALL %s, [",
                        i.result.name.c_str(), i.name.c_str()
                    );
                    for (auto& val : i.args) {
                        printf(" %s ", val.name.c_str());
                    }
                    printf("]\n");
                    break;
                }

                case INSTR_TAG::UNARY: {
                    printf("%s = %s%s\n", i.result.name.c_str(), i.op.c_str(), val_to_str(i.lhs).c_str());
                    break;
                }

                default: {
                    throw std::runtime_error(std::format("DUMP_TAC: Cannot display instr '{}'.", i.name));
                }
            }
        }
    }
}

std::string TacGenVisitor::val_to_str(Value val)
{
    switch (val.type) {
        case VALUE_TYPE::CONST: {
            return std::to_string(val.constVal);
        }

        case VALUE_TYPE::TEMP: {
            return val.name;
        }

        case VALUE_TYPE::VAR: {
            return val.name;
        }
    }

    return "???";
}

void TacGenVisitor::visit(NumberNode& n) 
{
    Value t = new_temp_val();
    Instr i;
    i.tag = INSTR_TAG::CONST;
    i.result = t;
    i.lhs = Value::Const(n.get_val());
    push_to_func_body(i);
    _result = t;
}

void TacGenVisitor::visit(BinaryExpression& exp) 
{
    Value l = gen_expr(exp.get_left());
    Value r = gen_expr(exp.get_right());

    Value t = new_temp_val();
    Instr i;

    i.tag = INSTR_TAG::BINOP;
    i.result = t;

    i.lhs = l;
    i.rhs = r;

    i.op = exp.get_op();

    push_to_func_body(i);

    _result = t;
}

void TacGenVisitor::visit(UnaryNode& n)
{
    Value val = gen_expr(n.get_val());
    Value t = new_temp_val();
    Instr i;

    i.tag = INSTR_TAG::UNARY;
    i.result = t;

    i.lhs = val;
    i.op = n.get_op();

    push_to_func_body(i);

    _result = t;
}

void TacGenVisitor::visit(VariableRefNode& n)
{
    Value t = new_temp_val();
    Instr i;
    i.tag = INSTR_TAG::LOAD;
    i.result = t;

    i.name = n.get_name();

    push_to_func_body(i);
    _result = t;
}

void TacGenVisitor::visit(ReturnStatement& n)
{
    Instr i;
    i.tag = INSTR_TAG::RET;
    if (n.get_return_val())
        i.lhs = gen_expr(n.get_return_val());
    push_to_func_body(i);
}

void TacGenVisitor::visit(BlockNode& n) {
    for (auto& s : n.get_stmts())
        s->accept(*this);
}

void TacGenVisitor::visit(FuncDeclNode& n)
{
    TacFunc tf;
    tf.name = n.get_name();
    tf.retType = "int";
    for (auto& arg : n.get_args())
    {
        tf.params.push_back((Param){
            arg->get_type()->get_type_name(), 
            arg->get_name(),
            arg->get_def_val() != nullptr
        });
    }

    _prog.funcs.push_back(std::move(tf));
    _curr_func = &_prog.funcs.back();

    // for (auto& arg : n.get_args())
    // {
    //     Instr i;
    //     i.tag = INSTR_TAG::DECL_VAR;
    //     i.name = arg->get_name();
    //     i.decl_type = arg->get_type()->get_type_name();
        
    //     if (arg->get_def_val()) 
    //     {
    //         i.lhs = gen_expr(arg->get_def_val());
    //     }
    //     else
    //     {
    //         i.lhs = Value::Const(0);
    //     }

    //     push_to_func_body(i);
    // }

    if (n.get_block())
    {
        n.get_block()->accept(*this);
    }
}

void TacGenVisitor::visit(RootNode& root) {
    for (auto& f : root.get_funcs())
        f->accept(*this);
}

void TacGenVisitor::visit(VariableAssignNode& n)
{
    Instr i;
    i.tag = INSTR_TAG::STORE;

    i.name = n.get_var_name();
    i.lhs = gen_expr(n.get_value());

    push_to_func_body(i);

    // throw std::runtime_error("TAC: VarAssignNode is not impl yet.");
}

void TacGenVisitor::visit(VariableDeclNode& n)
{
    Instr i;
    i.tag = INSTR_TAG::DECL_VAR;

    i.name = n.get_name();
    i.decl_type = n.get_type()->get_type_name();
    if (n.get_init()) 
    {
        i.lhs = gen_expr(n.get_init());
    }
    push_to_func_body(i);
    
    // throw std::runtime_error("TAC: VarDeclNode is not impl yet.");
}

void TacGenVisitor::visit(IfStatementNode& n) 
{
    std::string else_label_name = new_label();
    std::string end_label_name = new_label();

    // IF STMT
    Instr i;
    i.tag = INSTR_TAG::IF_FALSE; // IF_FALSE <cond>, <branch>
    i.lhs = gen_expr(n.get_condition()); // <cond>

    i.name = else_label_name;
    push_to_func_body(i);

    // THEN BLOCK
    n.get_then_block()->accept(*this);

    // SKIP ELSE
    Instr jmp;
    jmp.tag = INSTR_TAG::JUMP;
    jmp.name = end_label_name;
    push_to_func_body(jmp);

    Instr else_lb;
    else_lb.tag = INSTR_TAG::LABEL;
    else_lb.name = else_label_name;
    push_to_func_body(else_lb);

    if (n.get_else_block())
    {
        n.get_else_block()->accept(*this);
    }

    Instr end_lb;
    end_lb.tag = INSTR_TAG::LABEL;
    end_lb.name = end_label_name;
    push_to_func_body(end_lb);

    // throw std::runtime_error("TAC: If is not impl yet."); 
}

void TacGenVisitor::visit(FunctionCallNode& n)
{
    Value t = new_temp_val();
    Instr i;
    i.tag = INSTR_TAG::CALL;
    i.name = n.get_name();
    i.result = t;

    for (auto& arg : n.get_call_args())
    {
        i.args.push_back(gen_expr(arg));
    }

    push_to_func_body(i);
    _result = t;    

    // throw std::runtime_error("TAC: Call is not impl yet.");
}
void TacGenVisitor::visit(GotoStatement& n) 
{
    Instr i;
    i.tag = INSTR_TAG::JUMP;
    i.name = n.get_name();

    push_to_func_body(i);

    // throw std::runtime_error("TAC: Goto is not impl yet."); 
}
void TacGenVisitor::visit(LabelStatement& n)
{
    Instr i;
    i.tag = INSTR_TAG::LABEL;
    i.name = n.get_name();

    push_to_func_body(i);

    // throw std::runtime_error("TAC: Label is not impl yet.");
}

void TacGenVisitor::visit(ArgNode& n)
{
    // пустой метод, мы парсим прямо в родительском методе, через геттеры

    // throw std::runtime_error("TAC: Arg is not impl yet.");
}

void TacGenVisitor::visit(ExpressionStatement& exp)
{
    gen_expr(exp.get_expr());
}

void TacGenVisitor::visit(TypeNode&)
{
    // пустой метод, мы парсим прямо в родительском методе, через геттеры

    // throw std::runtime_error("TAC: Type is not impl yet.");
}
void TacGenVisitor::visit(ExternStatement&) 
{
    throw std::runtime_error("TAC: Extern is not impl yet.");
}