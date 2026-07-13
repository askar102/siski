#pragma once

#include <vector>
#include <memory>
#include <string>

#include "visitor.h"

class Node {
protected:
    Node* _parent = nullptr;
    void pad(int indent) const { for (int i = 0; i < indent; ++i) printf("  "); }
public:
    virtual ~Node() = default;

    void set_parent(Node* parent) { _parent = parent; }
    Node* get_parent() const { return _parent; }
    virtual void print(int indent = 0) const { pad(indent); printf("<node>\n"); }

    // visitor
    virtual void accept(Visitor& v) = 0;
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

    void print(int indent = 0) const override {
        pad(indent); printf("Type: %s%s\n", _type.c_str(), _is_pointer ? "*" : "");
    }

    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_type() { return _type; }
    bool is_pointer() { return _is_pointer; }
};

/* Argument node */
class ArgNode : public Node {
private:
    std::string _name;
    std::unique_ptr<TypeNode> _type;

    /* Default value */
    std::unique_ptr<ExpressionNode> _defaultValue;

public:
    ArgNode(std::string name, std::unique_ptr<TypeNode> type, std::unique_ptr<ExpressionNode> defVal)
        : _name(name), _type(std::move(type)), _defaultValue(std::move(defVal)) {}

    void print(int indent = 0) const override {
        pad(indent); 
        printf("Arg: %s\n", _name.c_str());

        if (_type) _type->print(indent + 1);

        if (_defaultValue) 
        {
            pad(indent + 1); printf("default:\n");
            _defaultValue->print(indent + 2);
        }
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_name() { return _name; }
    TypeNode* get_type() { return _type.get(); }
    ExpressionNode* get_def_val() { return _defaultValue.get(); }
};

/* Block node */
class BlockNode : public StatementNode {
private:
    std::vector<std::unique_ptr<StatementNode>> _stmts;
public:
    BlockNode(std::vector<std::unique_ptr<StatementNode>> stmts) 
        : _stmts(std::move(stmts)) {}

    void print(int indent = 0) const override {
        pad(indent); printf("Block:\n");
        for (auto& s : _stmts) s->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::vector<StatementNode*> get_stmts() {
        std::vector<StatementNode*> res;
        for (auto& ptr : _stmts) {
            res.push_back(ptr.get());
        }

        return res;
    }
};

/* Function node */
class FuncDeclNode : public DeclarationNode {
private:
    std::string _name;

    std::vector<std::unique_ptr<ArgNode>> _args;
    
    std::unique_ptr<TypeNode> _return_type;
    std::unique_ptr<BlockNode> _block;

public:
    FuncDeclNode(std::string name, std::vector<std::unique_ptr<ArgNode>> args, std::unique_ptr<TypeNode> return_type, std::unique_ptr<BlockNode> block)
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

    void print(int indent = 0) const override {
        pad(indent); printf("Func: %s\n", _name.c_str());
        for (auto& a : _args) a->print(indent + 1);
        if (_block) _block->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_name() { return _name; }

    std::vector<ArgNode*> get_args() {
        std::vector<ArgNode*> res;
        for (auto& ptr : _args) {
            res.push_back(ptr.get());
        }

        return res;
    }

    TypeNode* get_ret_type() { return _return_type.get(); } 
    BlockNode* get_block() { return _block.get(); }
};

class VariableDeclNode : public StatementNode {
private:
    std::unique_ptr<TypeNode> _type;
    std::string _var_name;
    std::unique_ptr<ExpressionNode> _init = nullptr;
public:
    VariableDeclNode(std::unique_ptr<TypeNode> type, std::string var_name, std::unique_ptr<ExpressionNode> init = nullptr)
        : _type(std::move(type)), _var_name(var_name), _init(std::move(init)) {}

    void print(int indent = 0) const override {
        pad(indent); printf("VarDecl: %s\n", _var_name.c_str());
        if (_init) _init->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    TypeNode* get_type() { return _type.get(); }
    std::string get_name() { return _var_name; }
    ExpressionNode* get_init() { return _init.get(); }
};

/* x = 1 */
class VariableAssignNode : public StatementNode {
private:
    std::string _var_name;
    std::unique_ptr<ExpressionNode> _value;
public:
    VariableAssignNode(std::string var_name, std::unique_ptr<ExpressionNode> value)
        : _var_name(var_name), _value(std::move(value)) {}

    void print(int indent = 0) const override {
        pad(indent); printf("Assign: %s\n", _var_name.c_str());
        if (_value) _value->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_var_name() { return _var_name; }
    ExpressionNode* get_value() { return _value.get(); }
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

    void print(int indent = 0) const override {
        pad(indent); printf("If:\n");
        pad(indent + 1); printf("cond:\n");  _condition->print(indent + 2);
        pad(indent + 1); printf("then:\n");  _then_block->print(indent + 2);
        if (_else_block) {
            pad(indent + 1); printf("else:\n");
            _else_block->print(indent + 2);
        }
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    ExpressionNode* get_condition() { return _condition.get(); }
    StatementNode* get_then_block() { return _then_block.get(); }
    StatementNode* get_else_block() { return _else_block.get(); }
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

    void print(int indent = 0) const override {
        pad(indent); 
        printf("Goto: %s\n", _label_name.c_str()); 
    }
    
    // visitor 
    void accept(Visitor& v) override { v.visit(*this); }


    std::string get_name() { return _label_name; }
};

class LabelStatement : public StatementNode {
private:
    std::string _label_name;
public:
    LabelStatement(std::string label_name)
        : _label_name(label_name) {}

    void print(int indent = 0) const override {
        pad(indent);
        printf("Label: %s\n", _label_name.c_str());
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_name() { return _label_name; }
};

class FunctionCallNode : public StatementNode {
private:
    std::string _func_name;
    std::vector<std::unique_ptr<ExpressionNode>> _call_args;

public:
    FunctionCallNode(std::string func_name, std::vector<std::unique_ptr<ExpressionNode>> call_args)
        : _func_name(func_name), _call_args(std::move(call_args)) {}

    void print(int indent = 0) const override {
        pad(indent); printf("Call: %s\n", _func_name.c_str());
        for (auto& a : _call_args) a->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_name() { return _func_name; }

    std::vector<ExpressionNode*> get_call_args() {
        std::vector<ExpressionNode*> res;
        for (auto& ptr : _call_args) {
            res.push_back(ptr.get());
        }

        return res;
    }
};

class NumberNode : public ExpressionNode {
private:
    int _val;
public:
    NumberNode(int val)
        : _val(val) {}

    void print(int indent = 0) const override {
        pad(indent); printf("Number: %d\n", _val);
    }   

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    int get_val() { return _val; }
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

    void print(int indent = 0) const override {
        pad(indent); printf("Binary: %s\n", _op.c_str());
        _left->print(indent + 1);
        _right->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    ExpressionNode* get_left() { return _left.get(); }
    std::string get_op() { return _op; }
    ExpressionNode* get_right() { return _right.get(); }
};

class VariableRefNode : public ExpressionNode {
private:
    std::string _var_name;
public:
    VariableRefNode(std::string var_name)
        : _var_name(var_name) {}

    void print(int indent = 0) const override {
        pad(indent); printf("VarRef: %s\n", _var_name.c_str());
    }

    // visitor 
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_name() { return _var_name; }
};

/* -1 */
class UnaryNode : public ExpressionNode {
private:
    std::string _op;
    std::unique_ptr<ExpressionNode> _val;

public:
    UnaryNode(std::string op, std::unique_ptr<ExpressionNode> val)
        : _op(op), _val(std::move(val)) {}

    void print(int indent = 0) const override {
        pad(indent); 
        printf("UnaryNumber: \n");
        pad(indent + 1);
        printf("Op: %s\n", _op.c_str());
        _val->print(indent + 1);
    }
    
    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_op() { return _op; }
    ExpressionNode* get_val() { return _val.get(); }
};

class ReturnStatement : public StatementNode {
private:
    std::unique_ptr<ExpressionNode> _return_val;
public:
    ReturnStatement(std::unique_ptr<ExpressionNode> return_val)
        : _return_val(std::move(return_val)) {}

    void print(int indent = 0) const override {
        pad(indent); printf("Return:\n");
        if (_return_val) _return_val->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    ExpressionNode* get_return_val() { return _return_val.get(); }
};

/* maybe */
class ExternStatement : public StatementNode {
private:
    std::string _row_code;
public:
    ExternStatement(std::string row_code)
        : _row_code(row_code) {}

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::string get_row_code() { return _row_code; }
};
 
/* Main/Root node */
class RootNode : public Node {
private:
    // todo: add function node  
    std::vector<std::unique_ptr<FuncDeclNode>> _funcs; 

public:
    RootNode(std::vector<std::unique_ptr<FuncDeclNode>> funcs) {
        _parent = this; 

        for (auto& func : funcs) {
            if (func) 
            {
                func->set_parent(this);
                _funcs.push_back(std::move(func));
            }
            
        }
    }

    void print(int indent = 0) const override {
        pad(indent); printf("Root:\n");
        for (auto& f : _funcs) f->print(indent + 1);
    }

    // visitor
    void accept(Visitor& v) override { v.visit(*this); }

    std::vector<FuncDeclNode*> get_funcs() {
        std::vector<FuncDeclNode*> res;
        for (auto& ptr : _funcs) {
            res.push_back(ptr.get());
        }

        return res;
    }
};