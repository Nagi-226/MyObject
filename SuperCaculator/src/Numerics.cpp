#include "Numerics.h"

#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>

using namespace std;

namespace Numerics {

static double getVar(const VarEnv& env, const std::string& name) {
    auto it = env.find(name);
    if (it == env.end()) throw runtime_error("Variable not set: " + name);
    return it->second;
}

static void setVar(VarEnv& env, const std::string& name, double v) {
    env[name] = v;
}

double partialDerivative(const Expression& f, VarEnv env, const std::string& varName) {
    double x = getVar(env, varName);
    double h = 1e-6 * max(1.0, fabs(x));

    setVar(env, varName, x + h);
    double f1 = f.eval(env);

    setVar(env, varName, x - h);
    double f2 = f.eval(env);

    return (f1 - f2) / (2.0 * h);
}

double integrateSimpson(const Expression& f, VarEnv env, const std::string& varName,
                        double a, double b, int n) {
    if (n < 2) n = 2;
    if (n % 2 == 1) n++;

    double h = (b - a) / n;

    setVar(env, varName, a);
    double s = f.eval(env);

    setVar(env, varName, b);
    s += f.eval(env);

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        setVar(env, varName, x);
        double fx = f.eval(env);
        s += (i % 2 == 0 ? 2.0 : 4.0) * fx;
    }

    return s * h / 3.0;
}

RootResult rootNewton(const Expression& f, VarEnv env, const std::string& varName,
                      double x0, int maxIter, double tol) {
    double x = x0;

    for (int i = 0; i < maxIter; i++) {
        env[varName] = x;
        double fx = f.eval(env);
        if (!isfinite(fx)) throw runtime_error("Non-finite f(x) in Newton");
        if (fabs(fx) < tol) return {x, i + 1};

        double dfx = partialDerivative(f, env, varName);
        if (!isfinite(dfx)) throw runtime_error("Non-finite f'(x) in Newton");
        if (fabs(dfx) < 1e-14) throw runtime_error("Derivative too small in Newton");

        double x1 = x - fx / dfx;
        if (fabs(x1 - x) < tol * max(1.0, fabs(x1))) return {x1, i + 1};
        x = x1;
    }

    throw runtime_error("Newton did not converge");
}

RootResult rootBisection(const Expression& f, VarEnv env, const std::string& varName,
                         double lo, double hi, int maxIter, double tol) {
    env[varName] = lo;
    double flo = f.eval(env);
    env[varName] = hi;
    double fhi = f.eval(env);

    if (!isfinite(flo) || !isfinite(fhi)) throw runtime_error("Non-finite endpoints in bisection");
    if (flo == 0.0) return {lo, 0};
    if (fhi == 0.0) return {hi, 0};
    if (flo * fhi > 0) throw runtime_error("Bisection requires opposite signs at endpoints");

    double a = lo, b = hi;
    for (int i = 0; i < maxIter; i++) {
        double m = (a + b) / 2.0;
        env[varName] = m;
        double fm = f.eval(env);
        if (!isfinite(fm)) throw runtime_error("Non-finite f(mid) in bisection");

        if (fabs(fm) < tol || fabs(b - a) < tol * max(1.0, fabs(m))) return {m, i + 1};

        if (flo * fm < 0) {
            b = m;
            fhi = fm;
        } else {
            a = m;
            flo = fm;
        }
    }

    throw runtime_error("Bisection did not converge");
}

RootResult rootSecant(const Expression& f, VarEnv env, const std::string& varName,
                      double x0, double x1, int maxIter, double tol) {
    double a = x0, b = x1;
    env[varName] = a;
    double fa = f.eval(env);
    env[varName] = b;
    double fb = f.eval(env);

    for (int i = 0; i < maxIter; i++) {
        if (!isfinite(fa) || !isfinite(fb)) throw runtime_error("Non-finite in secant");
        if (fabs(fb) < tol) return {b, i + 1};
        double denom = (fb - fa);
        if (fabs(denom) < 1e-14) throw runtime_error("Secant slope too small");
        double c = b - fb * (b - a) / denom;
        if (fabs(c - b) < tol * max(1.0, fabs(c))) return {c, i + 1};

        a = b; fa = fb;
        b = c;
        env[varName] = b;
        fb = f.eval(env);
    }

    throw runtime_error("Secant did not converge");
}

MinResult minimizeGolden(const Expression& f, VarEnv env, const std::string& varName,
                         double lo, double hi, int maxIter, double tol) {
    const double gr = (sqrt(5.0) - 1.0) / 2.0; // 0.618...
    double a = lo, b = hi;
    double c = b - gr * (b - a);
    double d = a + gr * (b - a);

    env[varName] = c;
    double fc = f.eval(env);
    env[varName] = d;
    double fd = f.eval(env);

    for (int i = 0; i < maxIter; i++) {
        if (fabs(b - a) < tol * max(1.0, fabs((a + b) / 2.0))) {
            double x = (a + b) / 2.0;
            env[varName] = x;
            return {x, f.eval(env), i + 1};
        }

        if (fc < fd) {
            b = d;
            d = c; fd = fc;
            c = b - gr * (b - a);
            env[varName] = c;
            fc = f.eval(env);
        } else {
            a = c;
            c = d; fc = fd;
            d = a + gr * (b - a);
            env[varName] = d;
            fd = f.eval(env);
        }
    }

    double x = (a + b) / 2.0;
    env[varName] = x;
    return {x, f.eval(env), maxIter};
}

vector<ODESample> solveODE_RK4(const Expression& dydx,
                              double x0, double y0,
                              double x1, int steps,
                              const std::string& xVar,
                              const std::string& yVar) {
    if (steps < 1) steps = 1;
    vector<ODESample> out;
    out.reserve((size_t)steps + 1);

    double h = (x1 - x0) / steps;
    double x = x0;
    double y = y0;

    VarEnv env;

    auto f = [&](double xx, double yy) {
        env[xVar] = xx;
        env[yVar] = yy;
        return dydx.eval(env);
    };

    out.push_back({x, y});

    for (int i = 0; i < steps; i++) {
        double k1 = f(x, y);
        double k2 = f(x + h / 2.0, y + h * k1 / 2.0);
        double k3 = f(x + h / 2.0, y + h * k2 / 2.0);
        double k4 = f(x + h, y + h * k3);

        y = y + (h / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
        x = x + h;
        out.push_back({x, y});
    }

    return out;
}

void exportCSV_1D(const Expression& f, VarEnv env, const std::string& varName,
                  double from, double to, int points,
                  const std::string& filePath) {
    if (points < 2) points = 2;

    ofstream ofs(filePath);
    if (!ofs) throw runtime_error("Cannot open file for writing: " + filePath);

    ofs << varName << ",value\n";
    for (int i = 0; i < points; i++) {
        double t = (points == 1) ? 0.0 : (double)i / (points - 1);
        double x = from + (to - from) * t;
        env[varName] = x;
        double y = f.eval(env);
        ofs << setprecision(17) << x << "," << y << "\n";
    }
}

} // namespace Numerics




