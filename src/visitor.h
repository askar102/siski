#pragma once

class NumberNode;
class BinaryExpression;
class UnaryNode;
class VariableRefNode;
class VariableDeclNode;
class VariableAssignNode;
class ReturnStatement;
class IfStatementNode;
class BlockNode;
class FunctionCallNode;
class GotoStatement;
class LabelStatement;
class FuncDeclNode;
class ArgNode;
class TypeNode;
class ExternStatement;
class ExpressionStatement;
class RootNode;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(NumberNode&) = 0;
    virtual void visit(BinaryExpression&) = 0;
    virtual void visit(UnaryNode&) = 0;
    virtual void visit(VariableRefNode&) = 0;
    virtual void visit(VariableDeclNode&) = 0;
    virtual void visit(VariableAssignNode&) = 0;
    virtual void visit(ReturnStatement&) = 0;
    virtual void visit(IfStatementNode&) = 0;
    virtual void visit(BlockNode&) = 0;
    virtual void visit(FunctionCallNode&) = 0;
    virtual void visit(GotoStatement&) = 0;
    virtual void visit(LabelStatement&) = 0;
    virtual void visit(FuncDeclNode&) = 0;
    virtual void visit(ArgNode&) = 0;
    virtual void visit(TypeNode&) = 0;
    virtual void visit(RootNode&) = 0;
    virtual void visit(ExternStatement&) = 0;
    virtual void visit(ExpressionStatement&) = 0;
};