#include "../../common_fast.h"

struct Context {

    enum Operator : int { ADD = 0, MUL, CONCAT };

    template <int OPCOUNT> struct Equation {
        Int64 result;
        Vector<Int64> terms;

        Int64 process() const {

            return tryCompute({});
        }

        Int64 tryCompute(const Vector<int> &ops) const {

            if (terms.size() == ops.size() + 1) {
                auto value = compute(ops);
                if (value == result) {
                    return value;
                }
            } else {
                for (int op = 0; op < OPCOUNT; ++op) {

                    auto copy = ops;
                    copy.push_back(op);
                    auto r = tryCompute(copy);

                    if (r) {
                        return r;
                    }
                }
            }

            return 0;
        };

        Int64 compute(const Vector<int> &_ops) const {
            auto current = terms[0];

            /* log << terms << endl; */

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
                        current = parseInt(std::to_string(a) + std::to_string(b));
                        break;
                }
            }

            return current;
        }
    };

    void part1(auto lines) {
        auto result{0_int64};

        auto to_equation = [](const auto &line) {
            Equation<2> eq;

            auto parts = line | rv::split_sv(' ');

            auto first = parts | rv::get0;

            eq.result = parseInt(first.substr(0, first.length() - 1));

            auto v = parts | rv::drop(1) | rv::to_ints;

            rs::copy(v, std::back_inserter(eq.terms));

            return eq;
        };

        auto compute = [](const auto &eq) { return eq.process(); };

        auto v = lines | rv::transform(to_equation) | rv::transform(compute);

        result = rs::fold_left(v, 0_int64, std::plus<>());

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        auto to_equation = [](const auto &line) {
            Equation<3> eq;

            auto parts = line | rv::split_sv(' ');

            auto first = parts | rv::get0;

            eq.result = parseInt(first.substr(0, first.length() - 1));

            auto v = parts | rv::drop(1) | rv::to_ints;

            rs::copy(v, std::back_inserter(eq.terms));

            return eq;
        };

        auto compute = [](const auto &eq) { return eq.process(); };

        auto v = lines | rv::transform(to_equation) | rv::transform(compute);

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
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
