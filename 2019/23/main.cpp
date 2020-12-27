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
    List<lli> inputs;
    int idleTime{0};

    void load(const Program &pgm) {
        program = pgm;
        cursor = 0;
        relativeBase = 0;
    }

    void addInput(const lli value) {
        inputs.push_back(value);
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

                    cursor += 2;

                    if (!inputs.empty()) {
                        a = inputs.front();
                        inputs.pop_front();
                        idleTime = 0;
                    } else {
                        a = -1;
                        idleTime++;
                        return -1;
                    }

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

struct Packet {
    lli x;
    lli y;
};

struct System {
    Program program;
    Array<Machine, 50> machines;
    Array<List<Packet>, 50> queues;

    Packet natPacket{-1, -1};

    void load(const String &line) {
        program.load(line);

        int i{0};
        for (auto &machine : machines) {
            machine.load(program);
            machine.addInput(i);
            machine.addInput(-1);
            ++i;
        }
    }

    void run() {
        bool doPart1{true};
        bool stop{false};

        lli lastReceived{-1};

        while (!stop) {
            bool allIdle{true};
            int m{0};
            for (auto &machine : machines) {
                auto &queue = queues[m];

                if (!queue.empty()) {
                    auto packet = queue.front();
                    queue.pop_front();
                    machine.addInput(packet.x);
                    machine.addInput(packet.y);

                    if (m == 0) {
                        if (packet.y == lastReceived) {
                            log << "Part2: " << packet.y << endl;
                            stop = true;
                            break;
                        } else {
                            lastReceived = packet.y;
                        }
                    }
                }

                lli r;

                r = machine.run();

                if (machine.idleTime < 10) {
                    allIdle = false;
                }

                if (r != -1) {
                    lli x = machine.run();
                    lli y = machine.run();

                    if (r == 255) {

                        if (doPart1) {
                            log << "Part1: " << y << endl;
                            doPart1 = false;
                        }

                        natPacket = {x, y};

                    } else {
                        auto &queue = queues[r];
                        queue.push_back({x, y});
                    }
                }

                ++m;
            }

            if (allIdle) {
                queues[0].push_back(natPacket);
            }
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
