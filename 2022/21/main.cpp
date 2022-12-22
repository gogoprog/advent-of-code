#include "../../common.h"

enum { VALUE, SUM, DIFF, MULT, DIV, CHECK, UNDEFINED };

char ops[] = {'V', '+', '-', '*', '/', '='};

struct Expression {
    String name;
    int type{UNDEFINED};

    lli value;
    String left;
    String right;

    lli compute(Map<String, Expression> &data) {
        auto &eleft = data[left];
        auto &eright = data[right];

        switch (type) {
            case VALUE:
                return value;
            case SUM:
                return eleft.compute(data) + eright.compute(data);
            case DIFF:
                return eleft.compute(data) - eright.compute(data);
            case DIV:
                return eleft.compute(data) / eright.compute(data);
            case MULT:
                return eleft.compute(data) * eright.compute(data);
            case CHECK:
                return eleft.compute(data) == eright.compute(data);
        }

        throw "shit";

        return 0;
    }

    bool directlyComputable(Map<String, Expression> &data) const {
        if (type != VALUE) {

            if (data[left].isValue() && data[right].isValue()) {
                return true;
            }
        }

        return false;
    }

    bool isValue() const {
        return type == VALUE;
    }

    void setValue(const lli v) {
        value = v;
        type = VALUE;
    }

    void print() {
        log << name << " : ";
        if (type == VALUE) {
            log << value;
        } else if (type == UNDEFINED) {
            log << "undefined";
        } else {
            log << left << " " << ops[type] << " " << right;
        }

        log << endl;
    }
};

struct Context {
    Map<String, Expression> expressions;
    Vector<Expression> allExpressions;

    Expression &get(const String name) {
        return expressions[name];
    }

    void parse(const Strings &lines) {
        String name, a, b, c;

        for (auto &line : lines) {
            InputStringStream iss{line};

            iss >> name;

            iss >> a;

            name.pop_back();

            if (!iss.eof()) {
                iss >> b >> c;
                auto &e = get(name);
                e.name = name;
                e.left = a;
                e.right = c;
                switch (b[0]) {
                    case '-':
                        e.type = DIFF;
                        break;
                    case '+':
                        e.type = SUM;
                        break;
                    case '/':
                        e.type = DIV;
                        break;
                    case '*':
                        e.type = MULT;
                        break;
                }
            } else {
                auto &e = get(name);
                e.name = name;
                e.type = VALUE;
                e.value = convert<String, int>(a);
            }
        }
    }

    void createInverses() {

        auto create_expression = [&](auto name) -> Expression & {
            allExpressions.push_back({name});

            return allExpressions.back();
        };

        for (auto kv : expressions) {
            auto &e = kv.second;

            switch (e.type) {
                case SUM: {
                    {
                        auto &ne = create_expression(e.left);
                        ne.type = DIFF;
                        ne.left = (e.name);
                        ne.right = (e.right);
                    }
                    {
                        auto &ne = create_expression(e.right);
                        ne.type = DIFF;
                        ne.left = (e.name);
                        ne.right = (e.left);
                    }
                } break;
                case DIFF: {
                    {
                        auto &ne = create_expression(e.left);
                        ne.type = SUM;
                        ne.left = (e.name);
                        ne.right = (e.right);
                    }
                    {
                        auto &ne = create_expression(e.right);
                        ne.type = DIFF;
                        ne.left = (e.left);
                        ne.right = (e.name);
                    }
                } break;
                case DIV: {
                    {
                        auto &ne = create_expression(e.left);
                        ne.type = MULT;
                        ne.left = (e.name);
                        ne.right = (e.right);
                    }
                    {
                        auto &ne = create_expression(e.right);
                        ne.type = DIV;
                        ne.left = (e.left);
                        ne.right = (e.name);
                    }
                } break;
                case MULT: {
                    {
                        auto &ne = create_expression(e.left);
                        ne.type = DIV;
                        ne.left = (e.name);
                        ne.right = (e.right);
                    }
                    {
                        auto &ne = create_expression(e.right);
                        ne.type = DIV;
                        ne.left = (e.name);
                        ne.right = (e.left);
                    }
                } break;
            }
        }
    }

    void part1() {
        lli result{0};

        result = get("root").compute(expressions);

        log << "Part1: " << result << endl;
    }

    void part2() {
        lli result{0};
        expressions.erase("humn");

        get("root").type = DIFF;
        createInverses();
        get("root").setValue(0);

        for (auto kv : expressions) {
            allExpressions.push_back(kv.second);
        }

        bool work = true;

        while (work) {
            work = false;

            for (auto &e : allExpressions) {
                if (e.directlyComputable(expressions)) {
                    e.setValue(e.compute(expressions));
                    expressions[e.name] = e;
                    work = true;
                }
            }
        }

        result = get("humn").value;

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
