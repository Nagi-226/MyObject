#include "Symbolic.h"

#include <cmath>
#include <sstream>
#include <stdexcept>

using namespace std;

namespace {

bool isConst(const ExprNodePtr& n, double* out = nullptr) {
    auto c = dynamic_pointer_cast<ConstNode>(n);
    if (!c) return false;
    if (out) *out = c->value;
    return true;
}

ExprNodePtr C(double v) { return make_shared<ConstNode>(v); }
ExprNodePtr V(const string& name) { return make_shared<VarNode>(name); }
ExprNodePtr U(char op, ExprNodePtr a) { return make_shared<UnaryNode>(op, std::move(a)); }
ExprNodePtr B(char op, ExprNodePtr a, ExprNodePtr b) { return make_shared<BinaryNode>(op, std::move(a), std::move(b)); }
ExprNodePtr F(const string& f, ExprNodePtr a) { return make_shared<FuncNode>(f, std::move(a)); }

ExprNodePtr simplifyOnce(const ExprNodePtr& n);

ExprNodePtr simplifyChildren(const ExprNodePtr& n) {
    if (auto u = dynamic_pointer_cast<UnaryNode>(n)) {
        return make_shared<UnaryNode>(u->op, simplifyOnce(u->child));
    }
    if (auto b = dynamic_pointer_cast<BinaryNode>(n)) {
        return make_shared<BinaryNode>(b->op, simplifyOnce(b->left), simplifyOnce(b->right));
    }
    if (auto f = dynamic_pointer_cast<FuncNode>(n)) {
        return make_shared<FuncNode>(f->fname, simplifyOnce(f->arg));
    }
    return n;
}

ExprNodePtr simplifyOnce(const ExprNodePtr& n) {
    auto m = simplifyChildren(n);

    // unary
    if (auto u = dynamic_pointer_cast<UnaryNode>(m)) {
        double cv;
        if (isConst(u->child, &cv)) {
            if (u->op == '-') return C(-cv);
        }
        // --a => a
        if (u->op == '-') {
            if (auto uu = dynamic_pointer_cast<UnaryNode>(u->child); uu && uu->op=='-') return uu->child;
        }
        return m;
    }

    // binary
    if (auto b = dynamic_pointer_cast<BinaryNode>(m)) {
        double a, c;
        bool lc = isConst(b->left, &a);
        bool rc = isConst(b->right, &c);

        if (lc && rc) {
            VarEnv env; // unused
            return C(b->eval(env));
        }

        // algebraic simplifications
        switch (b->op) {
            case '+':
                if (lc && a == 0.0) return b->right;
                if (rc && c == 0.0) return b->left;
                break;
            case '-':
                if (rc && c == 0.0) return b->left;
                if (lc && a == 0.0) return U('-', b->right);
                break;
            case '*':
                if ((lc && a == 0.0) || (rc && c == 0.0)) return C(0.0);
                if (lc && a == 1.0) return b->right;
                if (rc && c == 1.0) return b->left;
                break;
            case '/':
                if (lc && a == 0.0) return C(0.0);
                if (rc && c == 1.0) return b->left;
                break;
            case '^':
                if (rc && c == 0.0) return C(1.0);
                if (rc && c == 1.0) return b->left;
                if (lc && a == 0.0) return C(0.0);
                if (lc && a == 1.0) return C(1.0);
                break;
        }
        return m;
    }

    // function constant folding
    if (auto f = dynamic_pointer_cast<FuncNode>(m)) {
        double a;
        if (isConst(f->arg, &a)) {
            VarEnv env;
            return C(f->eval(env));
        }
        return m;
    }

    return m;
}

ExprNodePtr simplifyFixpoint(const ExprNodePtr& n) {
    ExprNodePtr cur = n;
    for (int i = 0; i < 50; i++) {
        auto nxt = simplifyOnce(cur);
        if (Symbolic::toString(nxt) == Symbolic::toString(cur)) return cur;
        cur = nxt;
    }
    return cur;
}

} // namespace

namespace Symbolic {

ExprNodePtr simplify(const ExprNodePtr& node) {
    if (!node) return node;
    return simplifyFixpoint(node);
}

ExprNodePtr derivative(const ExprNodePtr& node, const std::string& var) {
    if (!node) throw runtime_error("Null AST");

    if (dynamic_pointer_cast<ConstNode>(node)) {
        return C(0.0);
    }

    if (auto v = dynamic_pointer_cast<VarNode>(node)) {
        return C(v->name == var ? 1.0 : 0.0);
    }

    if (auto u = dynamic_pointer_cast<UnaryNode>(node)) {
        if (u->op == '-') return simplify(U('-', derivative(u->child, var)));
        throw runtime_error("Unsupported unary operator in derivative");
    }

    if (auto b = dynamic_pointer_cast<BinaryNode>(node)) {
        auto f = b->left;
        auto g = b->right;
        auto df = derivative(f, var);
        auto dg = derivative(g, var);

        switch (b->op) {
            case '+': return simplify(B('+', df, dg));
            case '-': return simplify(B('-', df, dg));
            case '*':
                // (fg)' = f'g + fg'
                return simplify(B('+', B('*', df, g), B('*', f, dg)));
            case '/':
                // (f/g)' = (f'g - fg') / g^2
                return simplify(B('/', B('-', B('*', df, g), B('*', f, dg)), B('^', g, C(2.0))));
            case '^': {
                // general: (f^g)' = f^g * ( g' * ln(f) + g * f'/f )
                // requires f>0 for ln(f) in real domain; we still build expression.
                auto term1 = B('*', dg, F("ln", f));
                auto term2 = B('*', g, B('/', df, f));
                return simplify(B('*', B('^', f, g), B('+', term1, term2)));
            }
            default:
                throw runtime_error("Unsupported binary operator in derivative");
        }
    }

    if (auto fn = dynamic_pointer_cast<FuncNode>(node)) {
        auto u = fn->arg;
        auto du = derivative(u, var);

        // chain rule
        if (fn->fname == "sin") return simplify(B('*', F("cos", u), du));
        if (fn->fname == "cos") return simplify(B('*', U('-', F("sin", u)), du));
        if (fn->fname == "tan") {
            // (tan u)' = u' / cos(u)^2
            return simplify(B('*', B('/', C(1.0), B('^', F("cos", u), C(2.0))), du));
        }
        if (fn->fname == "exp") return simplify(B('*', F("exp", u), du));
        if (fn->fname == "ln")  return simplify(B('*', B('/', C(1.0), u), du));
        if (fn->fname == "log") {
            // log10(u)' = u' / (u ln(10))
            return simplify(B('*', B('/', C(1.0), B('*', u, C(log(10.0)))), du));
        }
        if (fn->fname == "sqrt") {
            // (sqrt u)' = u' / (2*sqrt(u))
            return simplify(B('*', B('/', C(1.0), B('*', C(2.0), F("sqrt", u))), du));
        }
        if (fn->fname == "abs") {
            // derivative of abs is sign(u) (undefined at 0). We'll return u/abs(u) * u'
            return simplify(B('*', B('/', u, F("abs", u)), du));
        }

        throw runtime_error("Unsupported function in derivative: " + fn->fname);
    }

    throw runtime_error("Unknown AST node type in derivative");
}

static string toStringImpl(const ExprNodePtr& node, int parentPrec);

static int precOf(const ExprNodePtr& node) {
    if (dynamic_pointer_cast<ConstNode>(node) || dynamic_pointer_cast<VarNode>(node) || dynamic_pointer_cast<FuncNode>(node)) return 10;
    if (auto u = dynamic_pointer_cast<UnaryNode>(node)) return 9;
    if (auto b = dynamic_pointer_cast<BinaryNode>(node)) {
        switch (b->op) {
            case '+': case '-': return 1;
            case '*': case '/': return 2;
            case '^': return 3;
        }
    }
    return 0;
}

static string wrapIfNeeded(const string& s, bool need) {
    if (!need) return s;
    return "(" + s + ")";
}

static string toStringImpl(const ExprNodePtr& node, int parentPrec) {
    if (auto c = dynamic_pointer_cast<ConstNode>(node)) {
        ostringstream oss;
        oss.setf(std::ios::fixed);
        oss << setprecision(12) << c->value;
        string r = oss.str();
        // trim trailing zeros
        while (r.find('.') != string::npos && !r.empty() && r.back() == '0') r.pop_back();
        if (!r.empty() && r.back() == '.') r.pop_back();
        if (r.empty()) r = "0";
        return r;
    }
    if (auto v = dynamic_pointer_cast<VarNode>(node)) return v->name;
    if (auto f = dynamic_pointer_cast<FuncNode>(node)) {
        return f->fname + "(" + toStringImpl(f->arg, 0) + ")";
    }
    if (auto u = dynamic_pointer_cast<UnaryNode>(node)) {
        string s = string(1, u->op) + toStringImpl(u->child, precOf(node));
        return wrapIfNeeded(s, precOf(node) < parentPrec);
    }
    if (auto b = dynamic_pointer_cast<BinaryNode>(node)) {
        int p = precOf(node);
        string L = toStringImpl(b->left, p);
        // power is right-assoc; be conservative with parentheses on RHS
        string R = toStringImpl(b->right, (b->op=='^') ? (p-1) : p);
        string s = L + string(1, b->op) + R;
        return wrapIfNeeded(s, p < parentPrec);
    }
    return "?";
}

std::string toString(const ExprNodePtr& node) {
    return toStringImpl(node, 0);
}

} // namespace Symbolic




