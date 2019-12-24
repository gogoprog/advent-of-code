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
    Vector<int> inputs;
    int inputIndex;

    void load(const Program &pgm, const Vector<int> inputs = {}) {
        program = pgm;
        cursor = 0;
        relativeBase = 0;
        inputIndex = 0;
        this->inputs = inputs;
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

    lli run() {

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
                    a = inputs[inputIndex];
                    ++inputIndex;

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

struct System {
    Program program;
    Machine machine;

    void load(const String &line) {
        program.load(line);
        machine.load(program);
    }

    void run() {

        int count{0};

        for (int x = 0; x < 50; ++x) {
            for (int y = 0; y < 50; ++y) {
                machine.load(program, {x, y});
                auto r = machine.run();

                if (r == 1) {
                    ++count;
                }
            }
        }

        log << "part1: " << count << endl;
    }

    void run2() {
        Map<Point, bool> map;

        auto isBeam = [&](const Point &pos) {
            auto it = map.find(pos);
            if (it == map.end()) {
                machine.load(program, {pos.x, pos.y});
                auto r = machine.run();
                auto v = r == 1;
                map[pos] = v;
                return v;
            }

            return it->second;
        };

        int y = 3;
        int prevx = 10;
        bool found{false};
        while (!found) {
            int x = prevx + 3;
            while (true) {
                if (isBeam({x, y})) {
                    prevx = x;

                    if (isBeam({x - 99, y})) {
                        if (isBeam({x - 99, y + 99})) {
                            if (isBeam({x, y + 99})) {
                                auto pos = Point{x, y};
                                auto r = (x - 99) * 10000 + y;
                                log << "part2: " << r << endl;
                                found = true;
                                break;
                            }
                        }
                    }

                    break;
                }

                --x;
            }

            ++y;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];

    System sys;
    sys.load(line);
    sys.run();
    sys.run2();
}

int main() {
    process("input.txt");
    return 0;
}
