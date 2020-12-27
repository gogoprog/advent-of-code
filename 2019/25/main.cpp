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
    String inputs;
    int inputIndex;

    void load(const Program &pgm) {
        program = pgm;
        cursor = 0;
        relativeBase = 0;
        inputIndex = 0;
    }

    void setInput(const String _inputs) {
        inputs = _inputs;
        inputIndex = 0;
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

                    if (inputIndex >= inputs.size()) {
                        /* a = 0; */
                        return -60;
                    } else {
                        a = inputs[inputIndex];
                    }
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
    Map<Point, char> map;
    Point pos{0, 0};

    void load(const String &line) {
        program.load(line);
        machine.load(program);

        map[pos * 2] = '.';
    }

    void run() {
        lli r;
        String outputs;
        List<String> inputs;
        auto lines = getFileLines("commands.txt");
        for (auto &line : lines) {
            inputs.push_back(line);
        }

        while ((r = machine.run()) != -1) {
            if (r == -99)
                break;

            if (r == -60) {
                draw();
                log << "-> ";
                if (inputs.empty()) {
                    String str;
                    std::getline(std::cin, str);
                    doInput(str);
                } else {
                    log << inputs.front() << endl;
                    doInput(inputs.front());
                    inputs.pop_front();
                }
            } else {
                log << char(r);
                outputs += char(r);
            }
        }
    }

    void doInput(const String line) {
        machine.setInput(line + '\n');

        auto before = pos;

        if (line == "west") {
            pos.x -= 1;
        }
        if (line == "east") {
            pos.x += 1;
        }
        if (line == "north") {
            pos.y -= 1;
        }
        if (line == "south") {
            pos.y += 1;
        }

        map[(pos * 2 + before * 2) / 2] = ' ';

        map[pos * 2] = '.';
        log << pos << endl;
    }

    void draw() {
        log << "Map:" << endl;
        const auto [minPoint, maxPoint] = getMinMax(map);
        for (int y = minPoint.y - 1; y <= maxPoint.y + 1; y++) {
            for (int x = minPoint.x - 1; x <= maxPoint.x + 1; ++x) {
                if (y == minPoint.y - 1 || y == maxPoint.y + 1 || x == minPoint.x - 1 || x == maxPoint.x + 1) {
                    log << '#';
                } else {
                    if (x == pos.x * 2 && y == pos.y * 2) {
                        log << 'O';

                    } else {

                        char c = map[{x, y}];
                        if (c) {
                            log << c;
                        } else {
                            log << '#';
                        }
                    }
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

    System sys;
    sys.load(line);
    sys.run();
}

int main() {
    process("input.txt");
    return 0;
}
