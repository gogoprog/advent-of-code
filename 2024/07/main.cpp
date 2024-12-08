#include "../../common_fast.h"

struct Context {

    enum Operator : int { ADD = 0, MUL, CONCAT };

    struct Equation {
        Int64 result;
        mutable Int64 cache{0};

        Vector<Int64> terms;

        template <int OPCOUNT> Int64 process() const {
            if (cache)
                return cache;
            auto r = tryCompute2<OPCOUNT>(0, terms[0]);
            cache = r;
            return r;
        }

        template <int OPCOUNT> Int64 tryCompute2(const int index, const Int64 value) const {
            if (!value)
                return 0;

            if (terms.size() == index + 1) {
                if (value == result) {
                    return value;
                }

            } else {
                if (value && value <= result) {

                    for (int op = 0; op < OPCOUNT; ++op) {
                        auto newval = operation(value, op, terms[index + 1]);
                        auto r = tryCompute2<OPCOUNT>(index + 1, newval);

                        if (r) {
                            return r;
                        }
                    }
                }
            }

            return 0;
        };

        static inline Int64 operation(const Int64 a, int op, const Int64 b) {

            switch (op) {
                case ADD:
                    return a + b;
                case MUL:
                    return a * b;
                case CONCAT:
                    int n = 10;
                    for (int i = 1; i < 10; ++i) {
                        if (n > b) {
                            return a * n + b;
                        }

                        n *= 10;
                    }

                    break;
            }

            return 0;
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
