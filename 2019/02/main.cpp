#include "../../common.h"

using Program = Vector<int>;

struct Machine {
    Program initialProgram;
    Program program;
    int cursor{0};

    void load(const String line) {
        auto inputs = splitString(line, ',');

        initialProgram.clear();

        for (auto &input : inputs) {
            initialProgram.push_back(std::stoi(input));
        }
    }

    void reset() {
        cursor = 0;
        program = initialProgram;
    }

    int run(const int noun, const int verb) {
        reset();

        program[1] = noun;
        program[2] = verb;

        while (cursor < program.size()) {
            auto i = program[cursor];

            switch (i) {
                case 1:
                case 2: {
                    auto a = program[cursor + 1];
                    auto b = program[cursor + 2];
                    auto c = program[cursor + 3];

                    int r;

                    if (i == 1) {
                        r = program[a] + program[b];
                    } else {
                        r = program[a] * program[b];
                    }

                    program[c] = r;

                    cursor += 4;

                } break;

                case 99:
                    return program[0];
                    break;
                default:
                    cursor++;
            }

            cursor %= program.size();
        }

        return -1;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Machine machine;

    machine.load(lines[0]);

    log << machine.run(12, 2) << endl;

    for (int noun = 0; noun < 99; noun++) {
        for (int verb = 0; verb < 99; verb++) {

            auto r = machine.run(noun, verb);

            if (r == 19690720) {
                log << 100 * noun + verb << endl;
            }
        }
    }
}

int main() {
    process("input.txt");
    return 0;
}
