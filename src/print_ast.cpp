#include "ast.h"
#include "print_ast.h"

void PrintAst::visit(NumberNode& n) 
{
    pad(); 
    printf("Number: %d\n", n.get_val());
};

void PrintAst::visit(BinaryExpression& exp) 
{
    pad();
    printf("Binary: %s\n", exp.get_op().c_str());
    _indent++;
    exp.get_left()->accept(*this);
    exp.get_right()->accept(*this);
    _indent--;
};

void PrintAst::visit(UnaryNode& n) 
{
    pad();
    printf("Unary:\n");
    _indent++;
    pad(); printf("Op: %s\n", n.get_op().c_str());
    n.get_val()->accept(*this);
    _indent--;
};

void PrintAst::visit(VariableRefNode& n) 
{
    pad();
    printf("VarRef: %s\n", n.get_name().c_str());
};

void PrintAst::visit(VariableDeclNode& n) 
{
    pad();
    printf("VarDecl:\n");
    _indent++;
    n.get_type()->accept(*this);
    pad(); printf("Name: %s\n", n.get_name().c_str());
    if (n.get_init()) {
        pad(); printf("InitVal:\n");
        _indent++;
        n.get_init()->accept(*this);
        _indent--;
    }
    _indent--;
};

void PrintAst::visit(VariableAssignNode& n) 
{
    pad();
    printf("VarAssign: %s\n", n.get_var_name().c_str());
    if (n.get_value()) {
        _indent++;
        pad(); printf("AssignVal:\n");
        _indent++;
        n.get_value()->accept(*this);
        _indent--;
        _indent--;
    }
};

void PrintAst::visit(ReturnStatement& stmt) 
{
    pad();
    printf("Return:\n");
    if (stmt.get_return_val()) {
        _indent++;
        stmt.get_return_val()->accept(*this);
        _indent--;
    }
};


void PrintAst::visit(IfStatementNode& n) 
{
    pad();
    printf("If:\n");
    _indent++;

    pad(); printf("Cond:\n");
    _indent++;
    n.get_condition()->accept(*this);
    _indent--;

    pad(); printf("Then:\n");
    _indent++;
    n.get_then_block()->accept(*this);
    _indent--;

    if (n.get_else_block()) {
        pad(); printf("Else:\n");
        _indent++;
        n.get_else_block()->accept(*this);
        _indent--;
    }

    _indent--;
};

void PrintAst::visit(BlockNode& n) 
{
    pad();
    printf("Block:\n");
    _indent++;
    for (auto& s : n.get_stmts())
        s->accept(*this);
    _indent--;
};

void PrintAst::visit(FunctionCallNode& n) 
{
    pad();
    printf("Call: %s\n", n.get_name().c_str());
    _indent++;
    for (auto& a : n.get_call_args())
        a->accept(*this);
    _indent--;
};

void PrintAst::visit(GotoStatement& n) 
{
    pad();
    printf("Goto: %s\n", n.get_name().c_str());
};

void PrintAst::visit(LabelStatement& n) 
{
    pad();
    printf("Label: %s\n", n.get_name().c_str());
};

void PrintAst::visit(FuncDeclNode& n) 
{
    pad();
    printf("Func: %s\n", n.get_name().c_str());
    _indent++;

    if (n.get_ret_type()) {
        pad(); printf("ReturnType:\n");
        _indent++;
        n.get_ret_type()->accept(*this);
        _indent--;
    }

    for (auto& a : n.get_args())
        a->accept(*this);

    if (n.get_block())
        n.get_block()->accept(*this);

    _indent--;
};

void PrintAst::visit(ArgNode& n) 
{
    pad();
    printf("Arg: %s\n", n.get_name().c_str());
    _indent++;
    if (n.get_type())
        n.get_type()->accept(*this);
    if (n.get_def_val()) {
        pad(); printf("Default:\n");
        _indent++;
        n.get_def_val()->accept(*this);
        _indent--;
    }
    _indent--;
};

void PrintAst::visit(TypeNode& n) 
{
    pad();
    printf("Type: %s%s\n", n.get_type_name().c_str(), n.is_pointer() ? "*" : "");
};

void PrintAst::visit(RootNode& n) 
{
    pad();
    printf("Root:\n");
    _indent++;
    for (auto& f : n.get_funcs())
        f->accept(*this);
    _indent--;
};

void PrintAst::visit(ExternStatement& n) 
{
    pad();
    printf("Extern: %s\n", n.get_row_code().c_str());
};