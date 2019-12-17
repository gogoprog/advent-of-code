#include "../../common.h"

using lli = long long int;

struct Program : Map<lli, lli> {
    void load(const String line) {
        auto inputs = splitString(line, ',');

        clear();

        lli i = 0;
        for (auto &input : inputs) {
            (*this)[i] = std::stoi(input);
            i++;
        }
    }
};

struct Machine {
    Program program;
    lli cursor{0};
    lli relativeBase;

    void load(const Program &pgm) {
        program = pgm;
        cursor = 0;
        relativeBase = 0;
    }

    lli &getParam(const lli pos, const String modes) {
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

            case '2': {
                auto a = program[cursor + pos];
                return program[relativeBase + a];
            }
            default:
                throw "shit";
        }
    }

    lli run(const lli input) {

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

                    lli r;

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
                    cursor += 2;
                    return a;

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

                case 9: {
                    auto &a = getParam(1, modes);

                    relativeBase += a;

                    cursor += 2;

                } break;

                case 99:
                    return -99;
                    break;
                default:
                    cursor++;
            }

            cursor %= program.size();
        }

        return -99;
    }
};

struct Operator {
    Program program;
    Machine machine;

    void load(const String &line) {
        program.load(line);
        machine.load(program);
    }

    void run() {
        int r;
        do {
            r = machine.run(0);

            if(r > 0) {
                log << char(r);
            }

        } while (r != -99);
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];

    Operator op;
    op.load(line);
    op.run();
}

int main() {
    process("input.txt");
    return 0;
}
