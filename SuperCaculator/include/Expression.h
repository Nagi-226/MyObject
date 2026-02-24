#pragma once
#include <string>
#include <memory>
#include <map>
#include <vector>

// 多变量表达式求值环境
using VarEnv = std::map<std::string, double>;

// 抽象语法树基类
struct ExprNode {
    virtual ~ExprNode() = default;
    virtual double eval(const VarEnv& env) const = 0;
};

using ExprNodePtr = std::shared_ptr<ExprNode>;

// 常数
struct ConstNode : ExprNode {
    double value;
    explicit ConstNode(double v) : value(v) {}
    double eval(const VarEnv& env) const override { (void)env; return value; }
};

// 变量 x,y,z
struct VarNode : ExprNode {
    std::string name;
    explicit VarNode(std::string n) : name(std::move(n)) {}
    double eval(const VarEnv& env) const override;
};

// 一元运算
struct UnaryNode : ExprNode {
    char op; // currently only '-'
    ExprNodePtr child;
    UnaryNode(char o, ExprNodePtr c) : op(o), child(std::move(c)) {}
    double eval(const VarEnv& env) const override;
};

// 二元运算: + - * / ^
struct BinaryNode : ExprNode {
    char op;
    ExprNodePtr left, right;
    BinaryNode(char o, ExprNodePtr l, ExprNodePtr r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
    double eval(const VarEnv& env) const override;
};

// 函数调用: sin cos tan exp log ln sqrt abs
struct FuncNode : ExprNode {
    std::string fname;
    ExprNodePtr arg;
    FuncNode(std::string f, ExprNodePtr a)
        : fname(std::move(f)), arg(std::move(a)) {}
    double eval(const VarEnv& env) const override;
};

// 表达式类：负责从字符串解析为 AST，并且支持数值求值
class Expression {
public:
    Expression() = default;
    explicit Expression(const std::string& text);

    double eval(const VarEnv& env) const;

    const ExprNodePtr& ast() const { return root; }

    static Expression parse(const std::string& s);

private:
    ExprNodePtr root;
};




