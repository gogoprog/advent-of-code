#include "../../common_fast.h"

inline static Uint64 pow10(int n) {
    Uint64 result = 1;

    for (int i = 1; i < 16; ++i) {
        if (i - 1 == n)
            return result;
        result *= 10;
    }

    return result;
}

struct Context {

    Vector<Vector<Int64>> terms;
    Vector<char> operators;

    void parse(auto _lines) {
        Vector<StringView> lines;
        rs::copy(_lines, std::back_inserter(lines));

        auto len = std::ranges::size(lines);

        for (int l = 0; l < len - 1; ++l) {
            auto line = lines[l];

            auto v = line | rv::split_sv(' ') | rv::filter_empty | rv::to_ints;

            terms.push_back({});
            auto &last = terms.back();
            rs::copy(v, std::back_inserter(last));
        }

        {
            auto v = lines[len - 1] | rv::split_sv(' ') | rv::filter_empty;

            for (auto op : v) {
                operators.push_back(op[0]);
            }
        }
    }

    void part1() {
        auto result{0_int64};

        for (int i = 0; i < operators.size(); ++i) {
            auto op = operators[i];
            auto subresult = terms[0][i];

            for (int row = 1; row < terms.size(); ++row) {

                switch (op) {
                    case '*':
                        subresult *= terms[row][i];
                        break;
                    case '+':
                        subresult += terms[row][i];
                        break;
                }
            }

            result += subresult;
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};
        Grid grid(lines);

        char current_op = 0;
        Int64 subresult = 0;

        for (int c = 0; c < grid.width; ++c) {
            int val = 0;

            for (int r = 0; r < grid.height - 1; ++r) {
                char ch = grid.get({c, r});

                if (std::isdigit(ch)) {
                    val *= 10;
                    val += (ch - '0');
                }
            }

            if (!val) {
                current_op = 0;
                result += subresult;
            } else {
                if (!current_op) {
                    current_op = grid.get({c, grid.height - 1});
                    subresult = val;
                } else {
                    switch (current_op) {
                        case '*':
                            subresult *= val;
                            break;
                        case '+':
                            subresult += val;
                            break;
                    }
                }
            }
        }

        result += subresult;

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
        context.part1();
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
