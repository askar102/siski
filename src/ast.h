#pragma once

#include <vector>
#include <memory>
#include <string>

#include "types.h"

class Node {
protected:
    Node* _parent = nullptr;
public:
    virtual ~Node() = default;

    void set_parent(Node* parent) { _parent = parent; }
    Node* get_parent() const { return _parent; }
};

class TypeNode : public Node {
private:
    CompilerType _type;
public:
    TypeNode(CompilerType type) : _type(type) {}
};

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
    

class FuncNode : public Node {
private:
    std::string _name;

    std::vector<std::unique_ptr<Node>> _args;
    
    std::unique_ptr<TypeNode> _return_type;
    std::unique_ptr<Node> _block;

public:
    FuncNode(std::string name, std::vector<std::unique_ptr<Node>> args, std::unique_ptr<TypeNode> return_type, std::unique_ptr<Node> block)
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