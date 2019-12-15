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
        Map<Point, bool> walls;
        Point target{0, 0};

        const Vector<Point> &deltas = {Point{0, 0}, Point{0, 1}, Point{0, -1}, Point{-1, 0}, Point{1, 0}};

        struct Node {
            Machine machine;
            Point pos;
            int steps;

            bool operator<(const Node &other) const {
                return steps < other.steps;
            }
        };

        Map<Point, int> visitedSteps;

        int best = std::numeric_limits<int>::max();

        PriorityQueue<Node> q;
        q.push({machine, {0, 0}, 0});

        while (!q.empty()) {
            const auto node = q.top();
            const auto pos = node.pos;
            q.pop();

            auto &vs = visitedSteps[pos];
            if (vs != 0 && vs <= node.steps) {
                continue;
            } else {
                vs = node.steps;
            }

            for (int d = 1; d <= 4; d++) {
                const auto &delta = deltas[d];
                auto npos = Point{pos.first + delta.first, pos.second + delta.second};

                if (walls[npos] == false) {
                    auto clone = node.machine;
                    auto r = clone.run(d);

                    switch (r) {
                        case 0:
                            walls[npos] = true;
                            break;

                        case 1:
                            q.push({clone, npos, node.steps + 1});
                            break;

                        case 2:
                            target = npos;

                            if (node.steps + 1 < best) {
                                best = node.steps + 1;
                            }

                            break;
                    }
                }
            }
        }

        log << "part1: " << best << endl;

        {
            const auto [minPoint, maxPoint] = getMinMax(walls);

            Map<Point, bool> oxygenes;

            oxygenes[target] = true;

            auto changed = true;
            int i = 0;
            while (changed) {
                changed = false;
                auto new_oxygenes = oxygenes;
                for (int y = minPoint.second; y <= maxPoint.second; y++) {
                    for (int x = minPoint.first; x <= maxPoint.first; ++x) {
                        if (oxygenes[{x, y}]) {
                            for (int d = 1; d <= 4; d++) {
                                const auto &delta = deltas[d];
                                auto npos = Point{x + delta.first, y + delta.second};
                                if (walls[npos] == false) {
                                    if (oxygenes[npos] == false) {
                                        new_oxygenes[npos] = true;
                                        changed = true;
                                    }
                                }
                            }
                        }
                    }
                }
                ++i;
                oxygenes = new_oxygenes;
            }

            log << "part2: " << i - 1 << endl;
        }
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
