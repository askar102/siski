#pragma once

#include "visitor.h"
#include "ast.h"
#include "tac.h"

#include <string>

class TacGenVisitor : public Visitor {
private:
    TacProgram _prog;
    TacFunc* _curr_func;
    int32_t _temp_counter = 0;
    int _label_counter = 0;
    Value _result; 

    Value new_temp_val() { return Value::Temp("t" + std::to_string(_temp_counter++)); }
    std::string new_label() { return "L" + std::to_string(_label_counter++); }
    void push_to_func_body(Instr i) { _curr_func->body.push_back(std::move(i)); }

    Value gen_expr(ExpressionNode* e) { e->accept(*this); return _result; }
    
    static std::string val_to_str(Value val);

public:
    TacProgram gen(RootNode& root) {
        root.accept(*this);
        return std::move(_prog);
    }

    static void dump_tac(const TacProgram& prog);

    void visit(NumberNode&) override;
    void visit(BinaryExpression&) override;
    void visit(UnaryNode&) override;
    void visit(VariableRefNode&) override;

    void visit(ReturnStatement&) override;
    void visit(FuncDeclNode&) override;
    void visit(BlockNode&) override;
    void visit(RootNode&) override;

    void visit(VariableDeclNode&) override;
    void visit(VariableAssignNode&) override;
    void visit(IfStatementNode&) override;
    void visit(FunctionCallNode&) override;
    void visit(GotoStatement&) override;
    void visit(LabelStatement&) override;
    void visit(ArgNode&) override;
    void visit(TypeNode&) override;
    void visit(ExternStatement&) override;
    void visit(ExpressionStatement&) override;

};