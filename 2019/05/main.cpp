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

    int &getParam(const int pos, const String modes) {
        char mode = modes[modes.length() - pos];

        switch (mode) {
            case '0': {
                auto a = program[cursor + pos];
                return program[a];
            }
            case '1': {
                auto &a = program[cursor + pos];
                return a;
            }
            default:
                /* throw "shit"; */
                assert(false);
        }
    }

    int run(const int input) {
        reset();

        while (cursor < program.size()) {
            auto inst = program[cursor];

            auto ps = inst / 100;
            auto i = inst - ps * 100;

            String modes;

            if (ps == 0) {
                modes = "0000";
            } else {
                modes = std::to_string(ps);
                modes = "0000" + modes;
            }

            switch (i) {
                case 1:
                case 2: {
                    auto a = getParam(1, modes);
                    auto b = getParam(2, modes);
                    auto &c = getParam(3, modes);

                    int r;

                    if (i == 1) {
                        r = a + b;
                    } else {
                        r = a * b;
                    }

                    c = r;

                    cursor += 4;

                } break;

                case 3: {
                    auto &a = getParam(1, modes);
                    a = input;

                    cursor += 2;
                    break;
                }

                case 4: {

                    auto &a = getParam(1, modes);
                    log << a << endl;
                    cursor += 2;

                } break;

                case 5: {
                    auto &a = getParam(1, modes);
                    auto &b = getParam(2, modes);

                    if (a != 0) {
                        cursor = b;
                    } else {
                        cursor += 3;
                    }

                } break;
                case 6: {
                    auto &a = getParam(1, modes);
                    auto &b = getParam(2, modes);

                    if (a == 0) {
                        cursor = b;
                    } else {
                        cursor += 3;
                    }

                } break;
                case 7: {
                    auto &a = getParam(1, modes);
                    auto &b = getParam(2, modes);
                    auto &c = getParam(3, modes);

                    if (a < b) {
                        c = 1;
                    } else {
                        c = 0;
                    }

                    cursor += 4;

                } break;
                case 8: {
                    auto &a = getParam(1, modes);
                    auto &b = getParam(2, modes);
                    auto &c = getParam(3, modes);

                    if (a == b) {
                        c = 1;
                    } else {
                        c = 0;
                    }

                    cursor += 4;

                } break;

                case 99:
                    return -1;
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
    /* machine.load("3,0,4,0,99"); */
    /* machine.load("1002,4,3,4,33"); */

     machine.run(1);
    machine.run(5);
}

int main() {
    process("input.txt");
    return 0;
}
