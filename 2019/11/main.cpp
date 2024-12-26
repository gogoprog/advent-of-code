#include "../../common.h"

using lli = long long int;

struct Program : Map<lli, lli> {
    void load(const String line) {
        auto inputs = splitString(line, ',');

        clear();

        lli i = 0;
        for (auto &input : inputs) {
            (*this)[i] = std::stoll(input);
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
                assert(false);
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

                case 9: {
                    auto &a = getParam(1, modes);

                    relativeBase += a;

                    cursor += 2;

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

struct Robot {
    Program program;
    Machine brain;
    int dir{0};
    Point pos{0, 0};
    Map<Point, int> panels;

    void run(int startColor) {

        Point minPoint = {0, 0};
        Point maxPoint = {0, 0};

        panels[pos] = startColor;

        while (true) {
            int currCol = panels[pos];

            int color = brain.run(currCol);

            if (color == -1) {
                break;
            }

            panels[pos] = color;

            int move = brain.run(currCol);

            switch (move) {
                case 0:
                    dir--;
                    if (dir < 0)
                        dir += 4;
                    break;
                case 1:
                    dir++;
                    dir %= 4;
                    break;
            }

            switch (dir) {
                case 0:
                    pos.second--;
                    break;
                case 1:
                    pos.first++;
                    break;
                case 2:
                    pos.second++;
                    break;
                case 3:
                    pos.first--;
                    break;
            }

            minPoint.first = std::min(pos.first, minPoint.first);
            minPoint.second = std::min(pos.second, minPoint.second);
            maxPoint.first = std::max(pos.first, maxPoint.first);
            maxPoint.second = std::max(pos.second, maxPoint.second);
        }

        log << "part1: " << panels.size() << endl;

        if (startColor == 1) {
            for (int y = minPoint.second; y <= maxPoint.second; y++) {
                for (int x = minPoint.first; x <= maxPoint.first; ++x) {

                    if (panels[{x, y}] == 1) {
                        log << "#";
                    } else {
                        log << " ";
                    }
                }

                log << endl;
            }
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];

    {

        Robot robot;

        robot.program.load(line);
        robot.brain.load(robot.program);

        robot.run(0);
    }

    {

        Robot robot;

        robot.program.load(line);
        robot.brain.load(robot.program);

        robot.run(1);
    }
}

int main() {
    process("input.txt");
    return 0;
}
