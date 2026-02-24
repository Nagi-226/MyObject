#pragma once
#include "Expression.h"
#include <string>
#include <vector>

namespace Numerics {

// 数值偏导（中心差分），varName 如 "x"/"y"/"z"
double partialDerivative(const Expression& f, VarEnv env, const std::string& varName);

// 定积分：对 varName 在 [a,b] 上积分（Simpson）
double integrateSimpson(const Expression& f, VarEnv env, const std::string& varName,
                        double a, double b, int n = 1000);

struct RootResult {
    double x;
    int iterations;
};

// 一维求根（对 varName），常用给 f(var)=0。
RootResult rootNewton(const Expression& f, VarEnv env, const std::string& varName,
                      double x0, int maxIter = 50, double tol = 1e-10);
RootResult rootBisection(const Expression& f, VarEnv env, const std::string& varName,
                         double lo, double hi, int maxIter = 200, double tol = 1e-10);
RootResult rootSecant(const Expression& f, VarEnv env, const std::string& varName,
                      double x0, double x1, int maxIter = 100, double tol = 1e-10);

struct MinResult {
    double x;
    double fx;
    int iterations;
};

// 一维最小值搜索：黄金分割搜索（要求区间内近似单峰）
MinResult minimizeGolden(const Expression& f, VarEnv env, const std::string& varName,
                         double lo, double hi, int maxIter = 200, double tol = 1e-10);

// ODE: y' = f(x, y)  (使用变量名 xVar, yVar 传入)
struct ODESample {
    double x;
    double y;
};

std::vector<ODESample> solveODE_RK4(const Expression& dydx,
                                   double x0, double y0,
                                   double x1, int steps,
                                   const std::string& xVar = "x",
                                   const std::string& yVar = "y");

// CSV 导出：对某个变量采样（1D）。其他变量使用 env 固定值。
// 输出列: varName,value
void exportCSV_1D(const Expression& f, VarEnv env, const std::string& varName,
                  double from, double to, int points,
                  const std::string& filePath);

} // namespace Numerics




