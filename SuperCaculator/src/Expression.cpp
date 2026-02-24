#include "Expression.h"

#include <cctype>
#include <cmath>
#include <stdexcept>
#include <unordered_set>

using namespace std;

static double getConstValue(const std::string& name) {
    if (name == "pi") return acos(-1.0);
    if (name == "e") return exp(1.0);
    throw runtime_error("Unknown identifier: " + name);
}

double VarNode::eval(const VarEnv& env) const {
    auto it = env.find(name);
    if (it == env.end()) {
        if (name == "pi" || name == "e") return getConstValue(name);
        throw runtime_error("Variable not set: " + name);
    }
    return it->second;
}

double UnaryNode::eval(const VarEnv& env) const {
    double v = child->eval(env);
    if (op == '-') return -v;
    throw runtime_error("Unknown unary operator");
}

static double applyBinary(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: throw runtime_error("Unknown binary operator");
    }
}

double BinaryNode::eval(const VarEnv& env) const {
    return applyBinary(op, left->eval(env), right->eval(env));
}

static double applyFunc(const string& f, double a) {
    if (f == "sin") return sin(a);
    if (f == "cos") return cos(a);
    if (f == "tan") return tan(a);
    if (f == "exp") return exp(a);
    if (f == "log") return log10(a);
    if (f == "ln") return log(a);
    if (f == "sqrt") return sqrt(a);
    if (f == "abs") return fabs(a);
    throw runtime_error("Unknown function: " + f);
}

double FuncNode::eval(const VarEnv& env) const {
    double v = arg->eval(env);
    return applyFunc(fname, v);
}

// ---------------- Parser ----------------

namespace {

struct Tok {
    enum Type { NUM, ID, OP, LP, RP, COMMA, END } type;
    double num{};
    string text;
    char op{};
};

class Lexer {
public:
    explicit Lexer(string s) : src(std::move(s)) {}

    Tok next() {
        skipWs();
        if (i >= src.size()) return Tok{Tok::END};
        char c = src[i];

        if (isdigit((unsigned char)c) || c == '.') {
            size_t j = i;
            while (j < src.size() && (isdigit((unsigned char)src[j]) || src[j] == '.')) j++;
            if (j < src.size() && (src[j] == 'e' || src[j] == 'E')) {
                size_t k = j + 1;
                if (k < src.size() && (src[k] == '+' || src[k] == '-')) k++;
                while (k < src.size() && isdigit((unsigned char)src[k])) k++;
                j = k;
            }
            double v = stod(src.substr(i, j - i));
            i = j;
            Tok t; t.type = Tok::NUM; t.num = v; return t;
        }

        if (isalpha((unsigned char)c) || c == '_') {
            size_t j = i;
            while (j < src.size() && (isalnum((unsigned char)src[j]) || src[j] == '_')) j++;
            string name = src.substr(i, j - i);
            i = j;
            Tok t; t.type = Tok::ID; t.text = name; return t;
        }

        i++;
        if (c == '(') return Tok{Tok::LP};
        if (c == ')') return Tok{Tok::RP};
        if (c == ',') return Tok{Tok::COMMA};
        if (c=='+'||c=='-'||c=='*'||c=='/'||c=='^') { Tok t; t.type=Tok::OP; t.op=c; return t; }

        throw runtime_error(string("Unexpected character: ") + c);
    }

    size_t pos() const { return i; }

private:
    string src;
    size_t i = 0;

    void skipWs() {
        while (i < src.size() && isspace((unsigned char)src[i])) i++;
    }
};

static const unordered_set<string> kFuncs = {"sin","cos","tan","exp","log","ln","sqrt","abs"};

class Parser {
public:
    explicit Parser(const string& s) : lex(s) {
        cur = lex.next();
    }

    ExprNodePtr parseExpr() {
        auto n = parseAddSub();
        if (cur.type != Tok::END) {
            throw runtime_error("Unexpected token at position " + to_string(lex.pos()));
        }
        return n;
    }

private:
    Lexer lex;
    Tok cur;

    void consume(Tok::Type t) {
        if (cur.type != t) throw runtime_error("Unexpected token at position " + to_string(lex.pos()));
        cur = lex.next();
    }

    ExprNodePtr parseAddSub() {
        auto n = parseMulDiv();
        while (cur.type == Tok::OP && (cur.op == '+' || cur.op == '-')) {
            char op = cur.op;
            consume(Tok::OP);
            auto r = parseMulDiv();
            n = make_shared<BinaryNode>(op, n, r);
        }
        return n;
    }

    ExprNodePtr parseMulDiv() {
        auto n = parsePow();
        while (cur.type == Tok::OP && (cur.op == '*' || cur.op == '/')) {
            char op = cur.op;
            consume(Tok::OP);
            auto r = parsePow();
            n = make_shared<BinaryNode>(op, n, r);
        }
        return n;
    }

    ExprNodePtr parsePow() {
        // right associative
        auto n = parseUnary();
        if (cur.type == Tok::OP && cur.op == '^') {
            consume(Tok::OP);
            auto r = parsePow();
            n = make_shared<BinaryNode>('^', n, r);
        }
        return n;
    }

    ExprNodePtr parseUnary() {
        if (cur.type == Tok::OP && cur.op == '-') {
            consume(Tok::OP);
            return make_shared<UnaryNode>('-', parseUnary());
        }
        return parsePrimary();
    }

    ExprNodePtr parsePrimary() {
        if (cur.type == Tok::NUM) {
            double v = cur.num;
            consume(Tok::NUM);
            return make_shared<ConstNode>(v);
        }

        if (cur.type == Tok::ID) {
            string name = cur.text;
            consume(Tok::ID);

            // function call
            if (cur.type == Tok::LP && kFuncs.count(name)) {
                consume(Tok::LP);
                auto a = parseAddSub();
                consume(Tok::RP);
                return make_shared<FuncNode>(name, a);
            }

            // variable or constant
            return make_shared<VarNode>(name);
        }

        if (cur.type == Tok::LP) {
            consume(Tok::LP);
            auto n = parseAddSub();
            consume(Tok::RP);
            return n;
        }

        throw runtime_error("Expected primary at position " + to_string(lex.pos()));
    }
};

} // namespace

Expression::Expression(const std::string& text) {
    *this = parse(text);
}

Expression Expression::parse(const std::string& s) {
    Expression e;
    Parser p(s);
    e.root = p.parseExpr();
    return e;
}

double Expression::eval(const VarEnv& env) const {
    if (!root) throw runtime_error("Empty expression");
    return root->eval(env);
}



