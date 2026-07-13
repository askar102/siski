#include "tac_gen.h"
#include "ast.h"
#include "tac.h"
#include <stdexcept>
#include <format>

void TacGenVisitor::dump_tac(const TacProgram& prog)
{
    for (auto& fn : prog.funcs)
    {
        printf("FUNC %s:\n", fn.name.c_str());
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
    _temp_counter = 0;

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
    // Value t = new_temp_val();
    // Instr i;
    // i.tag = INSTR_TAG::STORE;
    // i.result = t;

    // i.lhs = Value::Var(n.get_var_name());
    // i.rhs = Value::Const(n.get_value());

    // push_to_func_body(i);
    // _result = t;
    throw std::runtime_error("TAC: VarAssignNode is not impl yet.");
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

void TacGenVisitor::visit(IfStatementNode&) 
{ 
    throw std::runtime_error("TAC: If is not impl yet."); 
}
void TacGenVisitor::visit(FunctionCallNode&)
{
    throw std::runtime_error("TAC: Call is not impl yet.");
}
void TacGenVisitor::visit(GotoStatement&) 
{ 
    throw std::runtime_error("TAC: Goto is not impl yet."); 
}
void TacGenVisitor::visit(LabelStatement&)
{
    throw std::runtime_error("TAC: Label is not impl yet.");
}

void TacGenVisitor::visit(ArgNode&)
{
    throw std::runtime_error("TAC: Arg is not impl yet.");
}
void TacGenVisitor::visit(TypeNode&)
{
    throw std::runtime_error("TAC: Type is not impl yet.");
}
void TacGenVisitor::visit(ExternStatement&) 
{
    throw std::runtime_error("TAC: Extern is not impl yet.");
}