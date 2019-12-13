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

struct Arcade {
    Program program;
    Machine machine;
    Map<Point, int> tiles;
    int score{0};
    Point minPoint = {0, 0};
    Point maxPoint = {0, 0};

    void load(const String line) {
        program.load(line);
        machine.load(program);
    }

    void run(const int coin) {

        bool playing = coin == 2;
        program[0] = coin;
        machine.load(program);
        int count{0};
        int input{0};

        int paddleX{0};
        int ballX{0};

        while (true) {
            int x = machine.run(input);
            if (x == -99) {
                break;
            }
            int y = machine.run(input);
            int v = machine.run(input);
            Point pos = {x, y};

            tiles[pos] = v;

            if (v == 2) {
                ++count;
            }
            if (x != -1) {
                minPoint.first = std::min(pos.first, minPoint.first);
                minPoint.second = std::min(pos.second, minPoint.second);
                maxPoint.first = std::max(pos.first, maxPoint.first);
                maxPoint.second = std::max(pos.second, maxPoint.second);

                if (v == 4) {
                    ballX = x;
                }
                if (v == 3) {
                    paddleX = x;
                }
            } else {
                if (y == 0) {
                    score = v;
                }
            }

            if (playing) {
                if (paddleX < ballX) {
                    input = 1;
                } else if (paddleX > ballX) {
                    input = -1;
                } else {
                    input = 0;
                }
            }
        }

        if (coin == 0) {
            log << "part1: " << count << endl;
        } else {
            log << "part2: " << score << endl;
        }
    }

    void draw() {
        for (int y = minPoint.second; y <= maxPoint.second; y++) {
            for (int x = minPoint.first; x <= maxPoint.first; ++x) {

                switch (tiles[{x, y}]) {
                    case 0:
                        log << ' ';
                        break;
                    case 1:
                        log << '#';
                        break;
                    case 2:
                        log << 'B';
                        break;
                    case 3:
                        log << '-';
                        break;
                    case 4:
                        log << 'o';
                        break;
                }
            }

            log << endl;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto line = lines[0];

    Arcade arcade;
    arcade.load(line);
    arcade.run(0);
    arcade.run(2);
}

int main() {
    process("input.txt");
    return 0;
}
