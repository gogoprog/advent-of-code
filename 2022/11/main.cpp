#include "../../common.h"

using BigInt = ull;

struct Operation {
    char op;
    int param;
};

struct Monkey {
    int id;
    Vector<BigInt> items;
    Operation operation;
    int divider;
    int true_target;
    int false_target;

    int inspections{0};

    BigInt apply(const BigInt input) {
        auto param = operation.param;

        if (operation.param == -1) {
            param = input;
        }
        switch (operation.op) {
            case '*':
                return input * param;
            case '+':
                return input + param;
            case '-':
                return input - param;
            case '/':
                return input / param;
        }

        return 0;
    }
};

template <bool divide> struct Context {
    Vector<Monkey> monkeys;

    void parse(const Vector<String> &lines) {
        int l{0};
        String str;

        auto parse_name = [&](const auto line) {
            InputStringStream iss{line};
            int id;
            iss >> str >> id;
            monkeys.push_back({id});
        };
        auto parse_items = [&](const auto line) {
            auto &monkey = monkeys.back();
            InputStringStream iss{line};
            iss >> str >> str;

            String rest;
            std::getline(iss, rest);

            auto items = splitString<int>(rest, ',');

            for (auto item : items) {
                monkey.items.push_back(item);
            }
        };
        auto parse_operation = [&](const auto line) {
            auto &monkey = monkeys.back();
            InputStringStream iss{line};
            iss >> str >> str >> str >> str;

            iss >> monkey.operation.op;

            iss >> str;

            if (str == "old") {
                monkey.operation.param = -1;
            } else {
                monkey.operation.param = convert<String, int>(str);
            }
        };
        auto parse_test = [&](const auto line) {
            auto &monkey = monkeys.back();
            InputStringStream iss{line};
            iss >> str >> str >> str;
            iss >> monkey.divider;
        };
        auto parse_true = [&](const auto line) {
            auto &monkey = monkeys.back();
            InputStringStream iss{line};
            iss >> str >> str >> str >> str >> str;
            iss >> monkey.true_target;
        };
        auto parse_false = [&](const auto line) {
            auto &monkey = monkeys.back();
            InputStringStream iss{line};
            iss >> str >> str >> str >> str >> str;
            iss >> monkey.false_target;
        };

        while (l < lines.size()) {
            parse_name(lines[l]);
            parse_items(lines[l + 1]);
            parse_operation(lines[l + 2]);
            parse_test(lines[l + 3]);
            parse_true(lines[l + 4]);
            parse_false(lines[l + 5]);
            l += 7;
        }
    }

    void round(int common_divider = 0) {
        for (auto &monkey : monkeys) {
            for (auto item : monkey.items) {

                monkey.inspections++;

                item = monkey.apply(item);

                if constexpr (divide) {
                    item /= 3;
                }

                if (common_divider) {
                    item %= common_divider;
                }

                /* log << item << endl; */
                if (item % monkey.divider == 0) {
                    monkeys[monkey.true_target].items.push_back(item);
                } else {
                    monkeys[monkey.false_target].items.push_back(item);
                }
            }
            monkey.items.resize(0);
        }
    }

    void part1() {

        for (auto i : rs::iota_view(0, 20)) {
            round(0);
        }

        auto r = monkeys | rv::transform([](auto m) { return m.inspections; }) | ra::sort | rv::reverse | rv::take(2) |
                 ra::reduce(1, std::multiplies<int>());
        log << "Part1: " << r << endl;
    }

    void part2() {

        auto common_divider = monkeys | rv::transform([](auto m) { return m.divider; }) |
                              ra::reduce(BigInt(1), std::multiplies<BigInt>());

        for (auto i : rs::iota_view(0, 10000)) {
            round(common_divider);
        }

        auto r = monkeys | rv::transform([](auto m) { return m.inspections; }) | ra::sort | rv::reverse | rv::take(2) |
                 ra::reduce(BigInt(1), std::multiplies<BigInt>());

        log << "Part2: " << r << endl;
    }

    Vector<int> getItemCounts() const {
        Vector<int> result;
        for (auto &monkey : monkeys) {
            result.push_back(monkey.items.size());
        }
        return result;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        Context<true> ctx;
        ctx.parse(lines);
        ctx.part1();
    }
    {
        Context<false> ctx;
        ctx.parse(lines);
        ctx.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
