#include "../../common.h"

struct Program : Vector<int> {
    void load(const String line) {
        auto inputs = splitString(line, ',');

        clear();

        for (auto &input : inputs) {
            push_back(std::stoi(input));
        }
    }
};

struct Machine {
    Program program;
    int cursor{0};
    bool phaseDone{false};

    void load(const Program &pgm) {
        program = pgm;
        cursor = 0;
        phaseDone = false;
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
                assert(false);
        }
    }

    int run(const int phase, const int input) {

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
                    if (!phaseDone) {
                        a = phase;
                        phaseDone = true;
                    } else {
                        a = input;
                    }

                    cursor += 2;
                    break;
                }

                case 4: {

                    auto &a = getParam(1, modes);
                    /* log << a << endl; */
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

struct Ship {
    Array<Machine, 5> amplifiers;
    Program program;

    void load(const String line) {
        program.load(line);
    }

    void reset() {
        for (auto &amp : amplifiers) {
            amp.load(program);
        }
    }

    int run(const Array<int, 5> &phases) {
        reset();
        int output = 0;
        for (int i = 0; i < phases.size(); i++) {
            output = amplifiers[i].run(phases[i], output);
        }

        return output;
    }

    int run2(const Array<int, 5> &phases) {
        reset();
        int output = 0;
        int lastoutput = 0;
        while (true) {
            for (int i = 0; i < phases.size(); i++) {
                int v = amplifiers[i].run(phases[i], output);
                if(v == -1) {
                    return lastoutput;
                }

                output = v;
            }

            lastoutput = output;
        }

        return output;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];
    Ship ship;
    ship.load(line);

    {

        int best = 0;

        Array<int, 5> phases = {0, 1, 2, 3, 4};
        do {
            int r = ship.run(phases);
            best = std::max(r, best);
        } while (std::next_permutation(phases.begin(), phases.end()));

        log << "part1: " << best << endl;
    }

    {

        int best = 0;

        Array<int, 5> phases = {5, 6, 7, 8, 9};
        do {
            int r = ship.run2(phases);
            best = std::max(r, best);
        } while (std::next_permutation(phases.begin(), phases.end()));

        log << "part2: " << best << endl;
    }
}

int main() {
    /* process("sample.txt"); */
    /* process("sample2.txt"); */
    process("sample3.txt");
    process("input.txt");
    return 0;
}
