#include "../../common.h"

using Operator = char;

struct Expr {
    String str;
    ull value;
    Vector<Expr> exprs;
    Vector<Operator> operators;

    ull compute() const {
        if (operators.size() == 0) {
            return value;
        }

        ull result{exprs[0].compute()};

        for (int i{0}; i < operators.size(); ++i) {
            auto op = operators[i];

            switch (op) {
                case '+':
                    result += exprs[i + 1].compute();
                    break;
                case '*':
                    result *= exprs[i + 1].compute();
                    break;
            }
        }

        return result;
    }

    void parse() {
        if (str.find(" ") == String::npos) {
            value = std::stoull(str);
            return;
        }

        for (int i{0}; i < str.length(); ++i) {
            char c = str[i];

            if (c == ' ') {
                continue;
            }

            if (c >= '0' && c <= '9') {
                exprs.push_back({String{c}});
                exprs.back().parse();
            } else if (c == '*' || c == '+') {
                operators.push_back(c);
            } else if (c == '(') {
                int level{1};
                int start{i};

                while (c != ')' || level > 0) {
                    i++;
                    c = str[i];

                    if (c == '(')
                        level++;
                    if (c == ')')
                        level--;
                }

                auto substr = str.substr(start + 1, i - start - 1);
                exprs.push_back({substr});
                exprs.back().parse();
            }
        }
    }

    void precedeAddition() {
        auto found{true};

        while (found && operators.size() > 1) {
            found = false;

            for (int i{0}; i < operators.size(); ++i) {
                auto op = operators[i];

                if (op == '+') {
                    found = true;

                    Expr new_expr{"hack"};
                    new_expr.exprs.push_back(exprs[i]);
                    new_expr.operators.push_back('+');
                    new_expr.exprs.push_back(exprs[i + 1]);

                    exprs[i] = new_expr;

                    operators.erase(operators.begin() + i);
                    exprs.erase(exprs.begin() + i + 1);
                    break;
                }
            }
        }

        for (auto &expr : exprs) {
            expr.precedeAddition();
        }
    }

    String toString() const {
        if (operators.size() < 1) {
            return std::to_string(value);
        }

        String res = "(" + exprs[0].toString();

        for (int i{0}; i < operators.size(); ++i) {

            res += operators[i];
            res += exprs[i + 1].toString();
        }

        res += ")";

        return res;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0ull;
    auto r2 = 0ull;

    for (auto &line : lines) {
        {
            Expr expr{line};
            expr.parse();
            auto t1 = expr.compute();
            r += t1;
        }
        {
            Expr expr{line};
            expr.parse();
            expr.precedeAddition();

            auto t2 = expr.compute();
            r2 += t2;
        }
    }

    log << "Part1: " << r << endl;
    log << "Part2: " << r2 << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
