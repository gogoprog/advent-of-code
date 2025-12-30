#include "../../common_fast.h"

using State = Array<int16_t, 10>;

inline void iteratorz(Function<void(const State &)> func, const int size, const int required, const State &limits) {
    // log << "iteratorz " << size << " | " << required << " " << limits << endl;

    State value;
    switch (size) {
        case 0:
            break;
        case 1:
            value[0] = limits[0];
            func(value);
            break;

        case 2:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                value[1] = required - i;

                if (value[1] >= 0)
                    func(value);
            }
            break;
        case 3:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    value[2] = required - i - j;
                    if (value[2] >= 0)
                        func(value);
                }
            }
            break;
        case 4:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        value[3] = required - i - j - k;
                        if (value[3] >= 0)
                            func(value);
                    }
                }
            }
            break;
        case 5:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        for (int l = 0; l <= limits[3]; ++l) {
                            value[3] = l;
                            value[4] = required - i - j - k - l;
                            if (value[4] >= 0)
                                func(value);
                        }
                    }
                }
            }
            break;
        case 6:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        for (int l = 0; l <= limits[3]; ++l) {
                            value[3] = l;
                            for (int m = 0; m <= limits[4]; ++m) {
                                value[4] = m;
                                value[5] = required - i - j - k - l - m;
                                if (value[5] >= 0)
                                    func(value);
                            }
                        }
                    }
                }
            }
            break;
        case 7:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        for (int l = 0; l <= limits[3]; ++l) {
                            value[3] = l;
                            for (int m = 0; m <= limits[4]; ++m) {
                                value[4] = m;
                                for (int n = 0; n <= limits[5]; ++n) {
                                    value[5] = n;
                                    value[6] = required - i - j - k - l - m - n;
                                    if (value[6] >= 0)
                                        func(value);
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 8:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        for (int l = 0; l <= limits[3]; ++l) {
                            value[3] = l;
                            for (int m = 0; m <= limits[4]; ++m) {
                                value[4] = m;
                                for (int n = 0; n <= limits[5]; ++n) {
                                    value[5] = n;
                                    for (int o = 0; o <= limits[6]; ++o) {
                                        value[6] = o;
                                        value[7] = required - i - j - k - l - m - n - o;
                                        if (value[7] >= 0)
                                            func(value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 9:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        for (int l = 0; l <= limits[3]; ++l) {
                            value[3] = l;
                            for (int m = 0; m <= limits[4]; ++m) {
                                value[4] = m;
                                for (int n = 0; n <= limits[5]; ++n) {
                                    value[5] = n;
                                    for (int o = 0; o <= limits[6]; ++o) {
                                        value[6] = o;
                                        for (int p = 0; p <= limits[7]; ++p) {
                                            value[7] = o;
                                            value[8] = required - i - j - k - l - m - n - o - p;
                                            if (value[8] >= 0)
                                                func(value);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 10:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    for (int k = 0; k <= limits[2]; ++k) {
                        value[2] = k;
                        for (int l = 0; l <= limits[3]; ++l) {
                            value[3] = l;
                            for (int m = 0; m <= limits[4]; ++m) {
                                value[4] = m;
                                for (int n = 0; n <= limits[5]; ++n) {
                                    value[5] = n;
                                    for (int o = 0; o <= limits[6]; ++o) {
                                        value[6] = o;
                                        for (int p = 0; p <= limits[7]; ++p) {
                                            value[7] = o;
                                            for (int q = 0; q <= limits[8]; ++q) {
                                                value[8] = q;
                                                value[9] = required - i - j - k - l - m - n - o - p - q;
                                                if (value[9] >= 0)
                                                    func(value);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        default:
            log << "ERROR! Unsupported size " << size << endl;
            exit(0);
    }
}

struct Machine {
    int lights;
    int size;
    Vector<Vector<int>> buttons;
    State joltage;
    int joltageLen;
    int buttonsLen;
    Vector<Vector<int8_t>> buttonsPerJolt;

    Vector<Pair<int, int>> sizeJolt;

    int press(const int state, const int button) const {
        int result = state;

        auto &values = buttons[button];

        for (auto v : values) {
            result ^= (1 << v);
        }

        return result;
    }

    inline bool press2(State &state, const int button) const {
        auto &values = buttons[button];

        for (auto v : values) {
            state[v]++;

            if (state[v] > joltage[v]) {
                return false;
            }
        }

        return true;
    }

    inline void press2quick(State &state, const int button) const {
        auto &values = buttons[button];

        for (auto v : values) {
            state[v]++;
        }
    }

    inline void multipress(State &state, const Vector<int8_t> &pressed_buttons, const State &counts) const {
        for (int i{0}; i < pressed_buttons.size(); ++i) {
            auto button = pressed_buttons[i];
            auto &values = buttons[button];

            for (auto v : values) {
                state[v] += counts[i];
            }
        }
    }

    State getInitialState() const {
        auto r = joltage;

        for (auto &v : r) {
            v = 0;
        }

        return r;
    }
};

void print(const int value, const int size) {
    log << '[';
    for (int i = 0; i < size; i++) {
        if (value & (1 << i)) {
            log << '#';
        } else {
            log << '.';
        }
    }
    log << ']' << endl;
}

struct Context {

    Vector<Machine> machines;

    void parse(auto lines) {
        for (auto line : lines) {
            machines.push_back({});
            auto &machine = machines.back();

            auto parts = line | rv::split_sv(' ');

            {
                auto indicator = parts | rv::get0;
                auto str = indicator.substr(1, indicator.length() - 2);
                for (int i = 0; i < str.length(); ++i) {
                    char c = str[i];
                    if (c == '#') {
                        machine.lights |= 1 << i;
                    }
                }

                machine.size = str.length();
            }

            auto rest = parts | rv::drop(1);

            for (auto part : rest) {
                auto str = part.substr(1, part.length() - 2);
                auto values = str | rv::split_sv(',') | rv::to_ints;
                if (part[0] == '(') {
                    machine.buttons.push_back({});
                    rs::copy(values, std::back_inserter(machine.buttons.back()));

                } else {
                    int i{0};
                    for (auto v : values) {
                        machine.joltage[i] = v;
                        ++i;
                    }

                    machine.joltageLen = i;
                }
            }

            machine.buttonsLen = machine.buttons.size();

            machine.buttonsPerJolt.resize(machine.joltageLen);

            for (auto b{0}; b < machine.buttonsLen; b++) {
                for (auto j : machine.buttons[b]) {
                    machine.buttonsPerJolt[j].push_back(b);
                }
            }

            int j{0};
            for (auto buttons : machine.buttonsPerJolt) {
                machine.sizeJolt.push_back({buttons.size(), j});
                j++;
            }

            // for (int j{0}; j < machine.joltageLen; ++j) {
            //     machine.sizeJolt.push_back({machine.joltage[j], j});
            // }

            std::sort(machine.sizeJolt.begin(), machine.sizeJolt.end());
        }
    }

    int resolve(const Machine &machine) {
        struct Node {
            int state;
            int steps;
        };

        Queue<Node> q;
        q.push({0, 0});

        int best{1000000000};

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (node.steps >= best)
                continue;

            if (node.state == machine.lights) {
                if (node.steps < best) {
                    best = node.steps;
                }
            } else {
                for (int i = 0; i < machine.buttonsLen; ++i) {
                    auto copy = node;
                    copy.state = machine.press(copy.state, i);
                    copy.steps++;
                    q.push(copy);
                }
            }
        }

        return best;
    }

    int resolve2(const Machine &machine, const State &target_state) {

        struct Node {
            State state;
            int16_t steps;

            bool operator<(const Node &other) const {
                return state < other.state;
                // return steps < other.steps;
            }
        };


        Queue<Node> q;
        // PriorityQueue<Node> q;
        q.push({machine.getInitialState(), 0});

        auto jlen = machine.joltage.size();

        log << "target: " << target_state << endl;
        log << "buttonsPerJolt: " << machine.buttonsPerJolt << endl;

        int16_t best{10000};

        Map<State, int> visited;

        State limits;

        while (!q.empty()) {
            // const auto node = q.top();
            const auto node = q.front();
            q.pop();

            // log << q.size() << endl;

            if (node.steps >= best)
                continue;

            auto &v = visited[node.state];
            if (v && v <= node.steps) {
                // log << "visited" << endl;
                continue;
            }

            v = node.steps;

            // log << node.state << endl;

            bool skip = false;

            for (int j{0}; j < machine.joltageLen; ++j) {
                if (node.state[j] > target_state[j]) {
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                for (int j{0}; j < machine.joltageLen; ++j) {
                    // int jj = machine.joltageLen - 1 - machine.sizeJolt[j].second;
                    int jj = machine.sizeJolt[j].second;
                    // int jj = j;

                    const auto &buttons = machine.buttonsPerJolt[jj];
                    auto left = target_state[jj] - node.state[jj];

                    int i{0};
                    for (auto b : buttons) {
                        limits[i] = left;
                        for (int jjj : machine.buttons[b]) {
                            limits[i] = std::min((int)limits[i], target_state[jjj] - node.state[jjj]);
                        }
                        ++i;
                    }

                    if (left > 0) {
                        // log << node.state << endl;
                        if (node.steps + left < best) {
                            iteratorz(
                                [&](const auto &values) {
                                    auto copy = node;
                                    machine.multipress(copy.state, buttons, values);
                                    copy.steps += left;
                                    q.push(copy);

                                    if (q.size() % 100000000 == 0) {

                                        log << j << " : " <<  copy.steps << " " << copy.state << " " << q.size() << endl;
                                    }
                                },
                                buttons.size(), left, limits);
                        }

                        skip = true;
                        break;
                    }
                }
            }

            if (!skip) {
                if (target_state == node.state) {
                    log << "found: " << node.steps << endl;

                    if (node.steps < best) {
                        best = node.steps;
                    }
                }
            }
        }

        log << "best = " << best << endl;

        return best;
    }

    void part1() {
        auto result{0_int64};

        for (auto &machine : machines) {
            result += resolve(machine);
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        for (auto &machine : machines) {
            State target = machine.joltage;

            int min = 100000;

            for (int j{0}; j < machine.joltageLen; j++) {
                if (target[j] < min) {
                    min = target[j] - 9;
                }
            }

            for (int j{0}; j < machine.joltageLen; j++) {
                // target[j] -= min;
            }

            result += resolve2(machine, target);
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    // process("sample.txt");
    // process("sample2.txt");
    process("input.txt");
    return 0;
}
