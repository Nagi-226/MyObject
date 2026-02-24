#pragma once
#include "Expression.h"
#include <string>

// 符号微分：基于 Expression 的 AST 进行解析求导
// 约定：仅支持对单变量/多变量中的某一个变量（x/y/z 或任意名字）求导。

namespace Symbolic {

// 返回新的 AST（未必完全化简，但包含基础化简规则）
ExprNodePtr derivative(const ExprNodePtr& node, const std::string& var);

// 将 AST 进行一定程度的化简（常量折叠、0/1 规则等）
ExprNodePtr simplify(const ExprNodePtr& node);

// 将 AST 转为可读字符串
std::string toString(const ExprNodePtr& node);

} // namespace Symbolic




