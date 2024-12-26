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
    String inputs;
    int inputIndex;

    void load(const Program &pgm, const String inputs = "none") {
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
                assert(false);
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

        Map<Point, bool> intersections;

        do {
            r = machine.run();

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
                auto [x, y] = scaffold.xy;
                result += x * y;
                intersections[scaffold] = true;
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

            log << moveStr << endl;

            struct Node {
                Array<Vector<int>, 3> funcs;
                Vector<int> routine;

                int totalVisited{0};
                int totalDistance{0};

                int value() const {
                    return funcs[0].size() + funcs[1].size() + funcs[2].size();
                }

                String routineStr() const {
                    String result;
                    bool first{true};
                    for (auto i : routine) {
                        if (!first) {
                            result += ',';
                        }
                        first = false;
                        result += char('A' + i);
                    }

                    return result;
                }

                String funcStr(const int f) const {
                    String result;
                    bool first{true};
                    for (auto i : funcs[f]) {
                        if (!first) {
                            result += ',';
                        }
                        first = false;
                        if (i < 11) {
                            result += std::to_string(i);
                        } else {
                            result += char(i);
                        }
                    }

                    return result;
                }

                void print() const {
                    log << "Routine: ";
                    for (auto i : routine) {
                        log << char('A' + i) << " ";
                    }
                    log << endl;

                    for (int i = 0; i < 3; i++) {

                        log << char('A' + i) << " = ";

                        for (int c : funcs[i]) {

                            if (c > 10) {
                                log << char(c);
                            } else {
                                log << c;
                            }

                            log << ",";
                        }
                        log << endl;
                    }
                }

                bool operator<(const Node &other) const {

                    if (totalVisited == other.totalVisited) {

                        if (totalDistance == other.totalDistance) {
                            return value() > other.value();
                        }
                        return totalDistance > other.totalDistance;
                    }

                    return totalVisited < other.totalVisited;
                }

                int process(const Point &start, Map<Point, bool> &intersections, Map<Point, char> &map) {
                    Map<Point, bool> visited;

                    Point pos = start;
                    int dir = N;

                    totalDistance = 0;

                    for (int f : routine) {
                        auto &func = funcs[f];

                        for (int c : func) {
                            if (c == 'R') {
                                dir = getDir(dir + 1);
                                auto &delta = deltas[dir];
                                auto npos = pos + delta;
                                if (map[npos] != '#') {
                                    return -1;
                                }
                            } else if (c == 'L') {
                                dir = getDir(dir - 1);
                                auto &delta = deltas[dir];
                                auto npos = pos + delta;
                                if (map[npos] != '#') {
                                    return -1;
                                }
                            } else {
                                auto v = c;

                                auto &delta = deltas[dir];

                                for (int i = 0; i < v; ++i) {
                                    pos = pos + delta;
                                    if (map[pos] != '#') {
                                        return -1;
                                    }

                                    visited[pos] = true;
                                    totalDistance++;
                                }

                                auto npos = pos + delta;
                                if (!intersections[pos]) {
                                    if (map[npos] == '#') {
                                        return -1;
                                    }
                                }
                            }
                        }
                    }

                    return visited.size();
                }
            };

            int totalScaffolds = scaffolds.size();

            log << "totalScaffolds = " << totalScaffolds << endl;

            PriorityQueue<Node> q;

            auto n = Node{{}, {0}};

            n.funcs[0] = {'L', 4, 'L', 4, 'L', 10, 'R', 4}; // found without computer
            n.funcs[1] = {'R', 4, 'L', 4, 'L', 4, 'R', 8}; // found without computer

            q.push(n);

            auto push = [&](Node &node) {
                node.totalVisited = node.process(start, intersections, map);
                if (node.totalVisited >= 0) {
                    q.push(node);
                }
            };

            int best = 0;
            Node bestNode;

            while (!q.empty()) {
                const auto node = q.top();
                q.pop();

                auto r = node.totalVisited;

                if (r >= 0) {

                    if (r == totalScaffolds) {
                        bestNode = node;
                        break;
                    }

                    if (node.routine.size() < 10) {

                        for (int i = 0; i < 3; ++i) {
                            const auto &func = node.funcs[i];
                            auto copy = node;
                            copy.routine.push_back(i);
                            push(copy);
                        }
                    }

                    for (auto f = 0; f < 3; ++f) {
                        if (std::find(node.routine.begin(), node.routine.end(), f) != node.routine.end()) {
                            const auto &func = node.funcs[f];

                            if (func.size() < 10) {

                                if (func.size() == 0 || func.back() <= 10) {
                                    auto copy = node;
                                    copy.funcs[f].push_back('R');
                                    push(copy);
                                }

                                if (func.size() == 0 || func.back() <= 10) {
                                    auto copy = node;
                                    copy.funcs[f].push_back('L');
                                    push(copy);
                                }

                                for (char c = 2; c <= 10; c++) {
                                    if (func.size() == 0 || func.back() > 10) {
                                        /* if (func.size() > 0 && func.back() > 10) { */
                                        auto copy = node;
                                        copy.funcs[f].push_back(c);
                                        push(copy);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            {
                program[0] = 2;

                Vector<String> strs;
                strs.push_back(bestNode.routineStr());
                for (int i = 0; i < 3; ++i) {
                    strs.push_back(bestNode.funcStr(i));
                }

                String inputs;

                for (auto &str : strs) {
                    inputs += str;
                    inputs += char(10);
                }

                inputs += "n";
                inputs += char(10);

                int r;
                machine.load(program, inputs);
                do {
                    r = machine.run();

                    if (r > 0) {
                        if (r < 256) {

                            log << char(r);
                        } else {
                            log << "part2: " << r << endl;
                        }
                    }

                } while (r != -99);
            }
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
