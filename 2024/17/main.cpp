#include "../../common_fast.h"

struct Context {

    struct Registers {
        union {
            struct {
                int a;
                int b;
                int c;
            };
            int values[3];
        };
    };

    struct State {
        Registers registers;
        int cursor{0};
        Vector<int8_t> output;

        int comboOperand(int operand) {
            if (operand <= 3) {
                return operand;
            }

            return registers.values[operand - 4];
        }

        void debug() {
            log << "---" << endl;
            log << "Register A: " << registers.a << endl;
            log << "Register B: " << registers.b << endl;
            log << "Register C: " << registers.c << endl;
            log << "Cursor: " << cursor << endl;

            log << "Output: ";

            for (auto o : output) {
                log << o << ',';
            }
            log << endl;
        }

        String getOutput() const {
            String result;
            bool first = true;
            for (auto o : output) {
                if (first) {
                    first = false;
                } else {
                    result += ',';
                }

                result += std::to_string(o);
            }

            return result;
        }
    };

    using Instruction = Function<void(int operand)>;
    struct Line {
        Instruction instr;
        int operand;
    };
    using Program = Vector<Line>;

    State initialState;
    State state;
    Program program;
    Array<Instruction, 8> instructions;

    void parse(auto lines) {
        initialState.registers.a = lines | rv::get0 | rv::split_sv(": ") | rv::get1 | rv::to_int;
        initialState.registers.b = lines | rv::get1 | rv::split_sv(": ") | rv::get1 | rv::to_int;
        initialState.registers.c = lines | rv::get2 | rv::split_sv(": ") | rv::get1 | rv::to_int;

        instructions[0] = [&](auto operand) {
            // adv
            auto num = state.registers.a;
            auto den = std::pow(2, state.comboOperand(operand));
            state.registers.a = num / den;
        };
        instructions[1] = [&](auto operand) {
            // bxl

            state.registers.b = state.registers.b ^ operand;
        };
        instructions[2] = [&](auto operand) {
            // bst

            state.registers.b = state.comboOperand(operand) % 8;
        };
        instructions[3] = [&](auto operand) {
            // jnz
            if (state.registers.a != 0) {
                state.cursor = operand - 1;
            }
        };
        instructions[4] = [&](auto operand) {
            // bxc
            state.registers.b = state.registers.b ^ state.registers.c;
        };
        instructions[5] = [&](auto operand) {
            // out
            auto v = state.comboOperand(operand) % 8;
            state.output.push_back(v);
            /* log << "Output: " << v << endl; */
        };
        instructions[6] = [&](auto operand) {
            // bdv
            auto num = state.registers.a;
            auto den = std::pow(2, state.comboOperand(operand));
            state.registers.b = num / den;
        };
        instructions[7] = [&](auto operand) {
            // cdv
            auto num = state.registers.a;
            auto den = std::pow(2, state.comboOperand(operand));
            state.registers.c = num / den;
        };

        auto v = lines | rv::get4 | rv::split_sv(": ") | rv::get1 | rv::split_sv(',') | rv::to_ints;
        int i = 0;

        for (auto n : v) {

            if (i & 1) {
                program.back().operand = n;
            } else {
                program.push_back({});
                program.back().instr = instructions[n];
            }

            ++i;
        }
    }

    void part1(auto lines) {
        auto result{0_int64};

        state = initialState;

        while (state.cursor < program.size()) {
            /* state.debug(); */
            auto &line = program[state.cursor];
            line.instr(line.operand);
            state.cursor++;
        }

        /* state.debug(); */

        log << "Part1: " << state.getOutput() << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

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
    process("sample4.txt");
    process("sample3.txt");
    process("sample.txt");
    process("input.txt");
    return 0;
}
