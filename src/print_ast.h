#pragma once

#include <cstdint>
#include <cstdio>

#include "visitor.h"

class PrintAst : public Visitor {
private:
    int32_t _indent = 0 ;
    void pad() 
    {
        for (int i = 0; i < _indent; ++i) printf(" ");
    }

public:
    void visit(NumberNode& n) override;
    void visit(BinaryExpression& exp) override;
    void visit(UnaryNode& n) override;
    void visit(VariableRefNode& n) override;
    void visit(VariableDeclNode& n) override;
    void visit(VariableAssignNode& n) override;
    void visit(ReturnStatement& st) override;
    void visit(IfStatementNode& st) override;
    void visit(BlockNode& n) override;
    void visit(FunctionCallNode& n) override;
    void visit(GotoStatement& st) override;
    void visit(LabelStatement& st) override;
    void visit(FuncDeclNode& n) override;
    void visit(ArgNode& n) override;
    void visit(TypeNode& n) override;
    void visit(RootNode& n) override;
    void visit(ExternStatement& st) override;
};