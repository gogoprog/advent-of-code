#include "../../common_fast.h"

inline static Uint64 pow2(int n) {
    Uint64 result = 1;
    for (int i = 1; i < 16; ++i) {
        if (i - 1 == n)
            return result;
        result *= 2;
    }

    return result;
}

struct Context {

    struct Registers {
        union {
            struct {
                Uint64 a;
                Uint64 b;
                Uint64 c;
            };
            Uint64 values[3];
        };
    };

    struct State {
        Registers registers;
        int cursor{0};
        Vector<int8_t> output;
        Uint64 lastA;
        Uint64 firstA;

        inline Uint64 comboOperand(int operand) const {
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

            log << "Output: " << output << endl;
            log << "FirstA: " << firstA << endl;
            log << "LastA: " << lastA << endl;
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

        inline int matchOutput(const Vector<int8_t> &other) const {
            int i = -1;
            for (i = 0; i < output.size(); ++i) {
                if (other[i] != output[i])
                    break;
            }
            return i;
        }

        inline bool matchOutput2(const Vector<int8_t> &other) const {
            for (int i = 0; i < output.size(); ++i) {
                if (other[i] != output[i])
                    return false;
            }
            return true;
        }

        inline bool matchOutputReverse(const Vector<int8_t> &other) const {
            for (int i = 0; i < output.size(); ++i) {
                if (output[i] != other[i + other.size() - output.size()])
                    return false;
            }
            return true;
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
    Vector<int8_t> source;
    Array<Instruction, 8> instructions;

    void parse(auto lines) {
        initialState.registers.a = lines | rv::get0 | rv::split_sv(": ") | rv::get1 | rv::to_int;
        initialState.registers.b = lines | rv::get1 | rv::split_sv(": ") | rv::get1 | rv::to_int;
        initialState.registers.c = lines | rv::get2 | rv::split_sv(": ") | rv::get1 | rv::to_int;

        instructions[0] = [&](auto operand) {
            // adv
            auto olda = state.registers.a;
            auto num = state.registers.a;
            auto den = pow2(state.comboOperand(operand));
            /* log << "Adv: " << num << " / " << den << endl; */
            /* log << "den/" << den << endl; */
            state.registers.a = num / den;

            if (state.firstA == 0) {
                state.firstA = olda;
            }

            if (state.registers.a != 0) {
                state.lastA = olda;
                /* log << olda << endl; */
            }
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
            auto den = pow2(state.comboOperand(operand));
            state.registers.b = num / den;
        };
        instructions[7] = [&](auto operand) {
            // cdv
            auto num = state.registers.a;
            auto den = pow2(state.comboOperand(operand));
            state.registers.c = num / den;
        };

        auto v = lines | rv::get4 | rv::split_sv(": ") | rv::get1 | rv::split_sv(',') | rv::to_ints;
        rs::copy(v, std::back_inserter(source));

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

    bool attempt(Uint64 start, int matching, int left, Uint64 &result) {
        Uint64 i = start;

        while (true) {
            state = initialState;

            state.registers.a = i;

            /* log << i << endl; */

            while (state.cursor < program.size()) {
                auto &line = program[state.cursor];
                line.instr(line.operand);
                state.cursor++;
            }

            /* state.debug(); */

            /* auto m = state.matchOutput(source); */

            int m = 0;

            if (state.matchOutputReverse(source)) {
                m = state.output.size();
            }

            if (m > matching) {
                if (left > 1) {
                    /* state.debug(); */
                    if (attempt(i * 8, m, left - (m - matching), result)) {
                        return true;
                    }
                }
            }

            if (state.output == source) {
                result = i;
                return true;
            }

            i++;
        }

        return false;
    }

    void part2(auto lines) {
        Uint64 result = 0;
        auto len = source.size();

        attempt(1, 0, len, result);

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
    process("sample5.txt");
    /* process("sample4.txt"); */
    /* process("sample3.txt"); */
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
