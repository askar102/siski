#pragma once

#include <vector>
#include <memory>

class Node {
protected:
    Node* _parent = nullptr;
public:
    virtual ~Node() = default;

    void set_parent(Node* parent) { _parent = parent; }
    Node* get_parent() const { return _parent; }
};


class RootNode : public Node {
private:
    // todo: add function node
    std::vector<std::unique_ptr<Node>> _funcs; 

public:
    RootNode(std::vector<std::unique_ptr<Node>> funcs) {
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