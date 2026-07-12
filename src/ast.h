#pragma once

#include <vector>
#include <memory>
#include <string>

class Node {
protected:
    Node* _parent = nullptr;
public:
    virtual ~Node() = default;

    void set_parent(Node* parent) { _parent = parent; }
    Node* get_parent() const { return _parent; }
};

/* То, что выполняется пошагово */
class StatementNode : public Node {};

/* обьявленния */
class DeclarationNode : public Node {};

/* типо ретурнит что то */
class ExpressionNode : public Node {};

/* Type node */
class TypeNode : public Node {
private:
    std::string _type;
    bool _is_pointer = false;
public:
    TypeNode(std::string type, bool is_pointer) : _type(type), _is_pointer(is_pointer) {}
};

/* Argument node */
class ArgNode : public Node {
private:
    std::string _name;
    std::unique_ptr<TypeNode> _type;

    /* Default value */
    std::unique_ptr<Node> _defaultValue;

public:
    ArgNode(std::string name, std::unique_ptr<TypeNode> type, std::unique_ptr<Node> defVal)
        : _name(name), _type(std::move(type)), _defaultValue(std::move(defVal)) {}
};

/* Block node */
class BlockNode : public StatementNode {
private:
    std::vector<std::unique_ptr<StatementNode>> _stmts;
public:
    BlockNode(std::vector<std::unique_ptr<StatementNode>> stmts) 
        : _stmts(std::move(stmts)) {}
};

/* Function node */
class FuncNode : public DeclarationNode {
private:
    std::string _name;

    std::vector<std::unique_ptr<ArgNode>> _args;
    
    std::unique_ptr<TypeNode> _return_type;
    std::unique_ptr<BlockNode> _block;

public:
    FuncNode(std::string name, std::vector<std::unique_ptr<ArgNode>> args, std::unique_ptr<TypeNode> return_type, std::unique_ptr<BlockNode> block)
        : _name(name), _return_type(std::move(return_type)), _block(std::move(block)) 
        {
            /* Params */
            for (auto& arg : args) 
            {
                if (arg) 
                {
                    arg->set_parent(this);
                    _args.push_back(std::move(arg));    
                } 
            }

            /* Return type */
            if (_return_type) 
            {
                _return_type->set_parent(this);
            }

            /* Block */
            if (_block)
            {
                _block->set_parent(this);
            }
        }
};

/* x = 1 */
class VariableAssignNode : public StatementNode {
private:
    std::string _var_name;
    std::unique_ptr<ExpressionNode> _value;
public:
    VariableAssignNode(std::string var_name, std::unique_ptr<ExpressionNode> value)
        : _var_name(var_name), _value(std::move(value)) {}
};

/* If (1 > 0) { ... }*/
class IfStatementNode : public StatementNode {
private:
    std::unique_ptr<ExpressionNode> _condition;
    std::unique_ptr<StatementNode> _then_block; /* ну типо blockNode же тоже стейтмент */
    std::unique_ptr<StatementNode> _else_block;

public:
    IfStatementNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<StatementNode> then_block, std::unique_ptr<StatementNode> else_block)
        : _condition(std::move(condition)), _then_block(std::move(then_block)), _else_block(std::move(else_block)) {}
};

// /* 1 == 1 */
// class ConditionalExpression : public Node {};

/* goto label; label: */
class GotoStatement : public StatementNode {
private:
    std::string _label_name;
public:
    GotoStatement(std::string label_name)
        : _label_name(label_name) {}
};

class LabelStatement : public StatementNode {
private:
    std::string _label_name;
public:
    LabelStatement(std::string label_name)
        : _label_name(label_name) {}
};



class FunctionCallNode : public StatementNode {
private:
    std::string _func_name;
    std::unique_ptr<ExpressionNode> _call_args;

public:
    FunctionCallNode(std::string func_name, std::unique_ptr<ExpressionNode> call_args)
        : _func_name(func_name), _call_args(std::move(_call_args)) {}
};

class NumberNode : public ExpressionNode {
private:
    int _val;
public:
    NumberNode(int val)
        : _val(val) {}
};

/* 1 + 1 */
class BinaryExpression : public ExpressionNode {
private:
    std::unique_ptr<ExpressionNode> _left;
    std::string _op;
    std::unique_ptr<ExpressionNode> _right;

public:
    BinaryExpression(std::unique_ptr<ExpressionNode> left, std::string op, std::unique_ptr<ExpressionNode> right)
        : _left(std::move(left)), _op(op), _right(std::move(right)) {}
};


class ReturnStatement : public StatementNode {
private:
    std::unique_ptr<ExpressionNode> _return_val;
public:
    ReturnStatement(std::unique_ptr<ExpressionNode> return_val)
        : _return_val(std::move(return_val)) {}
};

/* maybe */
class ExternStatement : public StatementNode {
private:
    std::string _row_code;
public:
    ExternStatement(std::string row_code)
        : _row_code(row_code) {}
};
 
/* Main/Root node */
class RootNode : public Node {
private:
    // todo: add function node  
    std::vector<std::unique_ptr<FuncNode>> _funcs; 

public:
    RootNode(std::vector<std::unique_ptr<FuncNode>> funcs) {
        _parent = this; 

        for (auto& func : funcs) {
            if (func) 
            {
                func->set_parent(this);
                _funcs.push_back(std::move(func));
            }
            
        }
    }
};