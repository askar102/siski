#pragma once

#include <cstdint>
#include <cstdio>

#include "visitor.h"

class PrintAst : public Visitor {
private:
    int32_t _ident = 0 ;
    void pad() 
    {
        for (int i = 0; i < _ident; ++i) printf(" ");
    }

public:
    void visit(NumberNode&) override;
    void visit(BinaryExpression&) override;
    void visit(UnaryNode&) override;
    void visit(VariableRefNode&) override;
    void visit(VariableDeclNode&) override;
    void visit(VariableAssignNode&) override;
    void visit(ReturnStatement&) override;
    void visit(IfStatementNode&) override;
    void visit(BlockNode&) override;
    void visit(FunctionCallNode&) override;
    void visit(GotoStatement&) override;
    void visit(LabelStatement&) override;
    void visit(FuncDeclNode&) override;
    void visit(ArgNode&) override;
    void visit(TypeNode&) override;
    void visit(RootNode&) override;
    void visit(ExternStatement&) override;
};