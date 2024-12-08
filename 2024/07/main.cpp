#include "../../common_fast.h"

struct Context {

    enum Operator : int { ADD = 0, MUL, CONCAT };

    struct Equation {
        Int64 result;
        Vector<Int64> terms;

        template <int OPCOUNT> Int64 process() const {
            return tryCompute<OPCOUNT>({});
        }

        /* Int64 process2() const { */
        /*     Queue<Vector<int>> q; */

        /*     q.push({}); */

        /*     while (!q.empty()) { */
        /*         const auto node = q.front(); */
        /*         q.pop(); */

        /*         if (node.size() == terms.size() - 1) { */
        /*             auto value = compute(node); */
        /*             if (value == result) { */
        /*                 return value; */
        /*             } */

        /*         } else { */

        /*             for (int op = 0; op < OPCOUNT; ++op) { */
        /*                 auto copy = node; */
        /*                 copy.push_back(op); */
        /*                 q.push(copy); */
        /*             } */
        /*         } */
        /*     } */

        /*     return 0; */
        /* } */

        template <int OPCOUNT> Int64 tryCompute(const Vector<int> &ops) const {

            if (terms.size() == ops.size() + 1) {
                auto value = compute<OPCOUNT>(ops);
                if (value == result) {
                    return value;
                }

            } else {
                for (int op = 0; op < OPCOUNT; ++op) {

                    auto copy = ops;
                    copy.push_back(op);
                    auto r = tryCompute<OPCOUNT>(copy);

                    if (r) {
                        return r;
                    }
                }
            }

            return 0;
        };

        template <int OPCOUNT> inline Int64 compute(const Vector<int> &_ops) const {
            auto current = terms[0];

            for (int i = 0; i < _ops.size(); ++i) {

                switch (_ops[i]) {
                    case ADD:
                        current += terms[i + 1];
                        break;
                    case MUL:
                        current *= terms[i + 1];
                        break;
                    case CONCAT:
                        auto a = current;
                        auto b = terms[i + 1];

                        int n = 10;
                        for (int i = 1; i < 10; ++i) {
                            if (n > b) {
                                current = current * n + b;
                                break;
                            }

                            n *= 10;
                        }

                        break;
                }

                if (current > result) {
                    return 0;
                }
            }

            return current;
        }
    };

    Vector<Equation> equations;

    void parse(auto lines) {
        auto to_equation = [](const auto &line) {
            Equation eq;

            auto parts = line | rv::split_sv(' ');

            auto first = parts | rv::get0;

            eq.result = first | rv::to_int;

            auto v = parts | rv::drop(1) | rv::to_ints;

            rs::copy(v, std::back_inserter(eq.terms));

            return eq;
        };

        rs::copy(lines | rv::transform(to_equation), std::back_inserter(equations));
    }

    void part1(auto lines) {
        auto result{0_int64};

        auto compute = [](const auto &eq) { return eq.template process<2>(); };

        auto v = equations | rv::transform(compute);

        result = rs::fold_left(v, 0_int64, std::plus<>());

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        auto compute = [](const auto &eq) { return eq.template process<3>(); };

        auto v = equations | rv::transform(compute);

        result = rs::fold_left(v, 0_int64, std::plus<>());

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
