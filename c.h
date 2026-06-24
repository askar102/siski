#pragma once

// LEXER -> PARSER -> AST -> IR(ASM-type) -> NASM

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() = default;

};

class ProgramExprAst : public ExprAST {

};


/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
public:
    NumberExprAST(uint32_t val) : _val(val) {}

private:
    uint32_t _val;
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
public:
    VariableExprAST(const std::string& name) : _name(name) {}

private:
    std::string _name;
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
    private:
        std::string _name;
        std::vector<std::string> _args;
      
    public:
        PrototypeAST(const std::string &Name, std::vector<std::string> Args)
          : _name(Name), _args(Args) {}
      
        const std::string &getName() const { return _name; }
    };

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(std::string op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
        : _op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

private:
    std::string _op;
    std::unique_ptr<ExprAST> LHS, RHS;
};

/// FunctionAST - This class represents a function definition itself.
/// у нее есть типо хедер и боди
class FunctionAST {
    std::unique_ptr<PrototypeAST> _proto;
    std::unique_ptr<ExprAST> _body;
  
  public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
      : _proto(std::move(Proto)), _body(std::move(Body)) {}

      std::unique_ptr<PrototypeAST>* GetProto() { return &_proto; }
      std::unique_ptr<ExprAST>* GetBody() { return &_body; }
  };



/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
    private:
        std::string _callee;
        std::vector<std::unique_ptr<ExprAST>> _args;
      
    public:
        CallExprAST(const std::string &Callee,
                    std::vector<std::unique_ptr<ExprAST>> Args)
          : _callee(Callee), _args(std::move(Args)) {}
    };
    
    
    
    class BlockAST {
    private:
        std::vector<std::unique_ptr<ExprAST>> _body;
    };
    

