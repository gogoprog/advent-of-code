#include "../../common.h"

using lli = long long int;

enum { N, W, S, E };

inline int getDir(int in) {
    if (in < 0)
        in += 4;
    return in % 4;
}

const Vector<Point> deltas = {Point{0, -1}, Point{1, 0}, Point{0, 1}, Point{-1, 0}};

struct Movement {
    char turn;
    int steps;
};

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
        int x = 0;
        int y = 0;

        Map<Point, char> map;
        Vector<Point> scaffolds;
        Point start;

        do {
            r = machine.run(0);

            if (r > 0) {
                log << char(r);

                if (r == 10) {
                    y++;
                    x = 0;
                } else {
                    map[{x, y}] = r;

                    if (r == '#') {
                        scaffolds.push_back({x, y});
                    }

                    if (r == '^') {
                        start = {x, y};
                    }

                    x++;
                }
            }

        } while (r != -99);

        auto result = 0;

        for (auto &scaffold : scaffolds) {
            int c = 0;
            for (auto &delta : deltas) {
                auto p = scaffold + delta;

                if (map[p] == '#') {
                    c++;
                }
            }

            if (c == 4) {
                auto [x, y] = scaffold;
                result += x * y;
            }
        }

        log << "part1: " << result << endl;

        {

            Vector<Movement> moves;
            Point pos{start};
            int currentDir = N;
            char turn = '?';
            bool cont = true;

            int steps = 0;
            while (cont) {
                auto p = pos + deltas[currentDir];

                if (map[p] == '#') {
                    steps++;
                    pos = p;
                } else {
                    if (steps) {
                        moves.push_back({turn, steps});
                        steps = 0;
                    }

                    {
                        auto newDir = getDir(currentDir - 1);
                        auto newP = pos + deltas[newDir];

                        if (map[newP] == '#') {
                            turn = 'L';
                            currentDir = newDir;

                        } else {
                            newDir = getDir(currentDir + 1);
                            newP = pos + deltas[newDir];
                            if (map[newP] == '#') {
                                turn = 'R';
                                currentDir = newDir;
                            } else {
                                cont = false;
                            }
                        }
                    }
                }
            }

            String moveStr;

            bool first = true;
            for (auto &move : moves) {
                if (!first) {
                    moveStr += ',';
                }
                first = false;
                moveStr += move.turn;
                moveStr += ',';
                moveStr += std::to_string(move.steps);
            }

            /* log << moveStr << endl; */

            struct Node {
                Point pos;
                int dir;
                Map<Point, bool> visited;
                String funcs[3];
                Vector<int> routine;

                String routineStr() const {
                    String result;
                    for (int i : routine) {
                        result += funcs[i];
                    }

                    return result;
                }

                bool operator<(const Node &other) const {
                    return routine.size() < other.routine.size();
                }
            };

            PriorityQueue<Node> q;
            q.push({start, N});

            while (!q.empty()) {
                const auto node = q.top();
                const auto pos = node.pos;
                q.pop();

                if (node.visited.size() < scaffolds.size()) {

                    for (auto f = 0; f < 3; ++f) {
                        const auto &func = node.funcs[f];
                        if (func.length() < 10) {
                            for (char c = '0'; c <= '9'; c++) {
                                auto copy = node;
                                copy.funcs[f].push_back(c);
                            }
                            {
                                auto copy = node;
                                copy.funcs[f].push_back('R');
                            }
                            {
                                auto copy = node;
                                copy.funcs[f].push_back('L');
                            }
                        }
                    }

                } else {
                    log << "reached" << endl;
                }
            }

            /* Node node; */
            /* node.funcs[0] = "L,4,L,4,L,10"; */
            /* node.funcs[1] = "R,4,R,4"; */
            /* node.funcs[2] = "L,10"; */

            /* node.routine = {0, 0, 1, 0, 2}; */

            /* log << node.routineStr() << endl; */
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
