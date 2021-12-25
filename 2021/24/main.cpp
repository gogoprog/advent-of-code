#include "../../common.h"

using Register = char;

struct Variable {
    Register reg{0};
    int64_t value{0};
};

using Instruction = std::function<void(Register, Variable)>;

struct ProgramLine {
    Instruction instruction;
    Register reg;
    Variable var;
};

using Program = Vector<ProgramLine>;

struct Context {
    Program program;
    Vector<String> lines;
    Map<String, Instruction> instructions;
    Map<Register, int64_t> registers;
    Queue<int> inputs;

    String inputStr;

    int64_t getValue(const Register reg) {
        return registers[reg];
    }

    int64_t getValue(const Variable var) {

        if (var.reg) {
            return registers[var.reg];
        }
        return var.value;
    }

    void setRegister(const Register reg, const int64_t value) {
        registers[reg] = value;
    }

    void init(const Vector<String> &lines) {

        instructions["inp"] = [&](auto reg, auto var) {
            auto value = inputs.front();
            inputs.pop();
            setRegister(reg, value);
        };
        instructions["add"] = [&](auto reg, auto var) {
            auto a = getValue(reg);
            auto b = getValue(var);
            setRegister(reg, a + b);
        };
        instructions["mul"] = [&](auto reg, auto var) {
            auto a = getValue(reg);
            auto b = getValue(var);
            setRegister(reg, a * b);
        };
        instructions["div"] = [&](auto reg, auto var) {
            auto a = getValue(reg);
            auto b = getValue(var);
            setRegister(reg, a / b);
        };
        instructions["mod"] = [&](auto reg, auto var) {
            auto a = getValue(reg);
            auto b = getValue(var);
            setRegister(reg, a % b);
        };
        instructions["eql"] = [&](auto reg, auto var) {
            auto a = getValue(reg);
            auto b = getValue(var);
            setRegister(reg, a == b ? 1 : 0);
        };

        this->lines = lines;

        for (auto &line : lines) {
            InputStringStream iss{line};
            String op, a, b;

            iss >> op >> a >> b;

            Register reg = a[0];
            Variable var;

            if (b[0] >= 'a' && b[0] <= 'z') {
                var.reg = b[0];
            } else {
                if (!b.empty()) {
                    var.value = std::stoi(b);
                }
            }

            program.push_back({instructions[op], reg, var});
        }
    }

    void reset() {
        while (!inputs.empty()) {
            inputs.pop();
        }
        registers['x'] = 0;
        registers['y'] = 0;
        registers['z'] = 0;
        registers['w'] = 0;
    }

    void addInput(const int value) {
        inputs.push(value);
    }

    void setInput(const ull value) {
        inputStr = std::to_string(value);

        for (auto c : inputStr) {
            addInput(c - '0');
        }
    }

    int findOffset(const int index) {
        int l = 0;
        int inps = 0;

        for (auto &line : lines) {

            if (line.find("inp") == 0) {
                if (index == inps) {
                    return l;
                }
                inps++;
            }

            l++;
        }

        throw "Nop";
        return -1;
    }

    void run(const int offset = 0, const int len = 512) {
        for (auto &line : program | rv::drop(offset) | rv::take(len)) {
            line.instruction(line.reg, line.var);
        }
    }

    void debug() {
        log << "Registers:" << endl;
        for (auto kv : registers) {
            log << "  " << kv.first << " = " << kv.second << endl;
        }
    }
};

void process(Context &ctx, const Array<Set<int>, 15> &true_zs, const Vector<int> &current) {

    auto index = current.size();

    if (index == 14) {
        log << "found: ";
        for (auto i : current)
            log << i;
        log << endl;
        return;
    }

    for (int i = 1; i <= 9; ++i) {

        ctx.registers['z'] = 0;

        for (auto n : current) {
            ctx.addInput(n);
        }

        ctx.addInput(i);

        ctx.run(0, 18 * (current.size() + 1));

        auto final_z = ctx.registers['z'];
        auto &true_z = true_zs[index + 1];
        if (true_z.find(final_z) != true_z.end()) {
            auto copy = current;
            copy.push_back(i);
            process(ctx, true_zs, copy);
        }
    }
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Context ctx;
    ctx.init(lines);

    {
        ull i = 99196997985942ull;

        ctx.reset();
        ctx.setInput(i);

        ctx.run();

        if (ctx.registers['z'] == 0) {
            log << "verified: " << i << endl;
        }
    }

    if (1) {

        Array<Set<int>, 15> possible_zs;

        Set<int> out_zs;
        /* Vector<Pair<int, int>> pairs; */
        Array<Vector<Pair<int, int>>, 14> all_pairs;
        possible_zs[0].insert(0);

        for (int d = 0; d < 14; d++) {
            log << "digit " << d + 1 << endl;
            auto offset = ctx.findOffset(d);

            auto &zs = possible_zs[d];
            out_zs.clear();
            auto &pairs = all_pairs[d];
            for (auto z : zs) {

                for (int i = 1; i <= 9; ++i) {
                    ctx.registers['z'] = z;
                    ctx.addInput(i);
                    ctx.run(offset, 18);

                    auto final_z = ctx.registers['z'];
                    out_zs.insert(final_z);
                    pairs.push_back({z, final_z});
                }
            }

            log << "out_zs: " << out_zs.size() << endl;

            std::swap(possible_zs[d + 1], out_zs);
        }

        if (possible_zs[14].find(0) != possible_zs[14].end()) {
            log << "found something" << endl;

            for (auto &pair : all_pairs[13]) {
                if (pair.second == 0) {
                    log << pair << endl;
                }
            }

            Set<int> expected;
            Set<int> next_expected;
            expected.insert(0);

            Array<Set<int>, 15> true_zs;

            true_zs[14].insert(0);

            for (int d = 13; d >= 0; d--) {

                log << "digit " << d + 1 << endl;
                next_expected.clear();
                for (auto &pair : all_pairs[d]) {
                    if (expected.find(pair.second) != expected.end()) {
                        next_expected.insert(pair.first);
                        true_zs[d].insert(pair.first);
                    }
                }

                std::swap(next_expected, expected);
                log << true_zs[d].size() << endl;
                /* log << true_zs[d] << endl; */
            }

            process(ctx, true_zs, {});
        }
    }
}

int main() {
    process("input.txt");
    /* process("last.txt"); */
    return 0;
}
