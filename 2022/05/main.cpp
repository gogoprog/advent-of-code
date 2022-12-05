#include "../../common.h"

template <bool part2 = false> struct Context {

    Vector<Vector<char>> stacks;
    bool initializing{true};

    bool parse(const String line) {
        bool result = false;

        if (initializing) {

            int i = 1;
            int stack_index = 0;

            while (i < line.length()) {
                char c = line[i];
                i += 4;

                if (c >= 'A' && c <= 'Z') {
                    while (stacks.size() <= stack_index) {
                        stacks.push_back({});
                    }
                    stacks[stack_index].push_back(c);
                    result = true;
                }

                stack_index++;
            }

            if (!result) {
                initializing = false;
                init();
            }
        } else {

            if (line != "") {
                InputStringStream iss{line};
                String word;
                int quantity;
                int from;
                int to;

                iss >> word >> quantity >> word >> from >> word >> to;

                if (part2) {
                    process2(quantity, from, to);
                } else {
                    process(quantity, from, to);
                }
            }
        }

        return result;
    }

    void init() {
        for (auto &stack : stacks) {
            std::reverse(stack.begin(), stack.end());
            log << stack << endl;
        }
    }

    void process(const int amount, const int from, const int to) {
        auto &from_stack = stacks[from - 1];
        auto &to_stack = stacks[to - 1];

        for (int i = 0; i < amount; ++i) {
            auto c = from_stack.back();
            from_stack.pop_back();
            to_stack.push_back(c);
        }
    }

    void process2(const int amount, const int from, const int to) {

        auto &from_stack = stacks[from - 1];
        auto &to_stack = stacks[to - 1];

        for (int i = from_stack.size() - amount; i < from_stack.size(); ++i) {
            auto c = from_stack[i];
            to_stack.push_back(c);
        }

        from_stack.resize(from_stack.size() - amount);
    }

    void print() {
        String result;

        for (auto &stack : stacks) {
            if (stack.size()) {

                result += stack.back();
            } else {
                result += "?";
            }
        }

        log << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        Context<false> ctx;

        for (auto line : lines) {

            ctx.parse(line);
        }

        ctx.print();
    }
    {

        Context<true> ctx;

        for (auto line : lines) {

            ctx.parse(line);
        }

        ctx.print();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
