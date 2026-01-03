#include "../../common_fast.h"

using State = Array<int16_t, 10>;
using Row = Vector<int>;
using Matrix = Vector<Row>;
using Solutions = Map<int, int>;

void rref(Matrix &A) {
    int n = A.size();
    int m = A[0].size();

    int prevPivot = 1;

    for (int k = 0; k < n && k < m - 1; ++k) {
        if (A[k][k] == 0) {
            for (int i = k + 1; i < n; ++i) {
                if (A[i][k] != 0) {
                    std::swap(A[k], A[i]);
                    break;
                }
            }
        }

        int pivot = A[k][k];
        if (pivot == 0)
            continue;

        for (int i = k + 1; i < n; ++i) {
            for (int j = k + 1; j < m; ++j) {
                A[i][j] = (pivot * A[i][j] - A[i][k] * A[k][j]) / prevPivot;
            }
            A[i][k] = 0;
        }

        prevPivot = pivot;
    }

    for (auto &row : A) {
        if (row.back() < 0) {
            for (auto &v : row) {
                v *= -1;
            }
        }
    }
}

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

void print(const auto &matrix) {
    for (auto &row : matrix) {

        for (auto v : row) {
            log << v << ", ";
        }

        log << endl;
    }
}

inline void iteratorz(Function<void(const State &)> func, const int size, const int required, const State &limits) {
    // log << "iteratorz " << size << " | " << required << " " << limits << endl;

    State value;
    switch (size) {
        case 0:
            break;
        case 1:
            value[0] = required;
            if (value[0] <= limits[0]) {
                if (value[0] >= 0)
                    func(value);
                else
                    break;
            }
            break;

        case 2:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                value[1] = required - i;

                if (value[1] <= limits[1]) {
                    if (value[1] >= 0)
                        func(value);
                    else
                        break;
                }
            }
            break;
        case 3:
            for (int i = 0; i <= limits[0]; ++i) {
                value[0] = i;
                for (int j = 0; j <= limits[1]; ++j) {
                    value[1] = j;
                    value[2] = required - i - j;
                    if (value[2] <= limits[2]) {
                        if (value[2] >= 0)
                            func(value);
                        else
                            break;
                    }
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

                        if (value[3] <= limits[3]) {
                            if (value[3] >= 0)
                                func(value);
                            else
                                break;
                        }
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
                            if (value[4] <= limits[4]) {
                                if (value[4] >= 0)
                                    func(value);
                                else
                                    break;
                            }
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
                                if (value[5] <= limits[5]) {
                                    if (value[5] >= 0)
                                        func(value);
                                    else
                                        break;
                                }
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
                                    if (value[6] <= limits[6]) {
                                        if (value[6] >= 0)
                                            func(value);
                                        else
                                            break;
                                    }
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
                                        if (value[7] <= limits[7]) {
                                            if (value[7] >= 0)
                                                func(value);
                                            else
                                                break;
                                        }
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
                                            value[7] = p;
                                            value[8] = required - i - j - k - l - m - n - o - p;

                                            if (value[8] <= limits[8]) {
                                                if (value[8] >= 0)
                                                    func(value);
                                                else
                                                    break;
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
                                            value[7] = p;
                                            for (int q = 0; q <= limits[8]; ++q) {
                                                value[8] = q;
                                                value[9] = required - i - j - k - l - m - n - o - p - q;
                                                if (value[9] <= limits[9]) {
                                                    if (value[9] >= 0)
                                                        func(value);
                                                    else
                                                        break;
                                                }
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
    Matrix matrix;
    Vector<bool> hasNeg;
    Vector<int> maxPerButton;

    int press(const int state, const int button) const {
        int result = state;

        auto &values = buttons[button];

        for (auto v : values) {
            result ^= (1 << v);
        }

        return result;
    }

    inline void multipress(State &state, const Vector<int8_t> &pressed_buttons, const State &counts) const {
        for (int i{0}; i < pressed_buttons.size(); ++i) {
            auto button = pressed_buttons[i];

            int j{0};
            for (auto &row : matrix) {
                state[j] += (counts[i] * row[button]);
                ++j;
            }
        }
    }

    inline void press(State &state, const int button) const {
        int j{0};
        for (auto &row : matrix) {
            state[j] += row[button];
            ++j;
        }
    }

    State getInitialState() const {
        auto r = joltage;

        for (auto &v : r) {
            v = 0;
        }

        return r;
    }

    State getTargetState() const {

        State result{};

        int i{0};
        for (auto &row : matrix) {

            result[i] = row.back();
            ++i;
        }

        return result;
    }

    void simplify() {
        // print(matrix);
        rref(matrix);
        // print(matrix);
        init();
    }

    void init() {

        buttons.clear();
        buttons.resize(buttonsLen);

        for (int r{0}; r < matrix.size(); r++) {
            for (int b{0}; b < buttonsLen; b++) {
                if (matrix[r][b] != 0) {
                    buttons[b].push_back(r);
                }
            }
        }

        buttonsPerJolt.resize(joltageLen);
        for (int j{0}; j < joltageLen; ++j) {
            buttonsPerJolt[j].resize(0);
        }

        for (auto b{0}; b < buttonsLen; b++) {

            for (int j{0}; j < joltageLen; ++j) {
                if (matrix[j][b] != 0) {
                    buttonsPerJolt[j].push_back(b);
                }
            }
        }
        int j{0};
        sizeJolt.resize(0);
        for (auto buttons : buttonsPerJolt) {
            sizeJolt.push_back({buttons.size(), j});
            j++;
        }

        std::sort(sizeJolt.begin(), sizeJolt.end());

        hasNeg.resize(joltageLen);

        for (int j{0}; j < joltageLen; ++j) {
            hasNeg[j] = false;
            for (auto b{0}; b < buttonsLen; b++) {
                if (matrix[j][b] < 0) {
                    hasNeg[j] = true;
                    break;
                }
            }
        }
    }

    int countUnknown(const int j, Solutions &solutions) const {
        int missing{0};

        for (auto b : buttonsPerJolt[j]) {
            if (solutions.find(b) == solutions.end()) {
                missing++;
            }
        }

        return missing;
    }

    bool solve(const int j, Map<int, int> &solutions) const {
        int missing{0};
        int unknown;
        int sum{0};

        for (auto b : buttonsPerJolt[j]) {
            if (solutions.find(b) == solutions.end()) {
                missing++;
                unknown = b;
            } else {
                sum += matrix[j][b] * solutions[b];
            }
        }

        if (missing > 1) {
            return false;
        }

        if (missing == 1) {
            auto result = (matrix[j][buttonsLen] - sum) / matrix[j][unknown];

            if (result < 0) {
                return false;
            }

            solutions[unknown] = result;
        }

        return true;
    }

    bool verify(Map<int, int> &solutions) const {

        for (int j{0}; j < joltageLen; ++j) {
            int sum{0};
            for (int b{0}; b < buttonsLen; ++b) {
                if (matrix[j][b] != 0) {
                    sum += matrix[j][b] * solutions[b];
                }
            }

            if (sum != matrix[j][buttonsLen]) {
                return false;
            }
        }

        return true;
    }
};

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

            machine.maxPerButton.resize(machine.buttonsLen);

            for (auto &v : machine.maxPerButton) {
                v = 10000;
            }

            machine.matrix.resize(machine.joltageLen);

            for (int j{0}; j < machine.joltageLen; ++j) {
                machine.matrix[j].resize(machine.buttonsLen + 1);
                machine.matrix[j].back() = machine.joltage[j];
            }

            for (int b{0}; b < machine.buttonsLen; b++) {
                for (auto j : machine.buttons[b]) {
                    machine.matrix[j][b] = 1;
                }
            }

            machine.init();

            for (int j{0}; j < machine.joltageLen; ++j) {
                for (int b{0}; b < machine.buttonsLen; ++b) {
                    if (machine.matrix[j][b] != 0) {
                        machine.maxPerButton[b] = std::min(machine.maxPerButton[b], (int)machine.joltage[j]);
                    }
                }
            }
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

    template <bool fast> int resolve2(const Machine &machine) {

        struct Node {
            State state;
            int16_t steps;

            bool operator<(const Node &other) const {
                return state > other.state;
                // return distance() < other.distance();
            }
        };

        auto target_state = machine.getTargetState();

        Queue<Node> q;
        q.push({machine.getInitialState(), 0});

        print(machine.matrix);

        log << "target: " << target_state << endl;
        // log << "buttonsPerJolt: " << machine.buttonsPerJolt << endl;

        int16_t best{10000};

        Map<State, int> visited;

        State limits;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (node.steps >= best)
                continue;

            auto &v = visited[node.state];
            if (v && v <= node.steps) {
                // log << "visited" << endl;
                continue;
            }

            v = node.steps;

            bool skip = false;

            for (int j{0}; j < machine.joltageLen; ++j) {
                if (!machine.hasNeg[j] && node.state[j] > target_state[j]) {
                    // log << "skipped: " << node.state << endl;
                    skip = true;
                    break;
                }
            }

            if (!skip) {

                // log << q.size() << " - " << node.state << endl;

                // for (int b{0}; b < machine.buttonsLen; ++b) {
                //     auto copy = node;
                //     machine.press(copy.state, b);
                //     copy.steps++;
                //     q.push(copy);
                // }
                for (int j{0}; j < machine.joltageLen; ++j) {
                    // int jj = j;
                    int jj = machine.sizeJolt[j].second;
                    const auto &buttons = machine.buttonsPerJolt[jj];

                    auto left = target_state[jj] - node.state[jj];

                    if (left > 0) {
                        if constexpr (fast) {

                            int i{0};
                            for (auto b : buttons) {
                                limits[i] = left;
                                for (int jjj : machine.buttons[b]) {
                                    limits[i] = std::min((int)limits[i], target_state[jjj] - node.state[jjj]);
                                }
                                ++i;
                            }

                            iteratorz(
                                [&](const auto &values) {
                                    auto copy = node;
                                    machine.multipress(copy.state, buttons, values);
                                    copy.steps += left;
                                    q.push(copy);
                                },
                                buttons.size(), left, limits);
                        } else {

                            if (!machine.hasNeg[jj]) {

                                int i{0};
                                for (auto b : buttons) {
                                    limits[i] = left;
                                    ++i;
                                }

                                iteratorz(
                                    [&](const auto &values) {
                                        auto copy = node;
                                        machine.multipress(copy.state, buttons, values);
                                        copy.steps += left;
                                        q.push(copy);
                                    },
                                    buttons.size(), left, limits);

                            } else {
                                for (auto b : buttons) {
                                    auto copy = node;
                                    machine.press(copy.state, b);
                                    copy.steps++;
                                    q.push(copy);
                                }
                            }
                        }

                        skip = true;
                        break;
                    }
                }
            }

            if (!skip) {
                if (target_state == node.state) {
                    if (node.steps < best) {
                        best = node.steps;
                        log << "found " << best << endl;
                    }
                }
            }
        }

        log << "best : " << best << endl;

        return best;
    }

    int resolveR(const Machine &machine, Solutions &start) {

        int result{100000};

        Solutions solutions;
        solutions = start;

        // log << "start: " << start << endl;
        while (true) {

            bool cont = true;

            while (cont) {
                cont = false;
                for (int j = 0; j < machine.joltageLen; ++j) {
                    auto unknowns = machine.countUnknown(j, solutions);

                    if (unknowns == 1) {
                        if (machine.solve(j, solutions)) {
                            cont = true;
                        } else {
                            return 1000000;
                            break;
                        }
                    }
                }
            }

            cont = true;

            while (cont) {
                cont = false;
                for (int j = 0; j < machine.joltageLen; ++j) {
                    auto unknowns = machine.countUnknown(j, solutions);

                    if (unknowns == 2) {
                        cont = true;
                        for (auto b : machine.buttonsPerJolt[j]) {
                            if (solutions.find(b) == solutions.end()) {
                                for (int i = 0; i <= machine.maxPerButton[b]; ++i) {
                                    auto copy = solutions;
                                    copy[b] = i;
                                    auto r = resolveR(machine, copy);
                                    if (r < result) {
                                        result = r;
                                    }
                                }

                                return result;
                                break;
                            }
                        }

                        break;
                    }
                }
            }

            for (int j = 0; j < machine.joltageLen; ++j) {
                auto unknowns = machine.countUnknown(j, solutions);

                if (unknowns == 3) {
                    for (auto b : machine.buttonsPerJolt[j]) {
                        if (solutions.find(b) == solutions.end()) {
                            for (auto b2 : machine.buttonsPerJolt[j]) {
                                if (solutions.find(b2) == solutions.end() && b2 != b) {

                                    for (int i = 0; i <= machine.maxPerButton[b]; ++i) {
                                        for (int j = 0; j <= machine.maxPerButton[b2]; ++j) {
                                            auto copy = solutions;
                                            copy[b] = i;
                                            copy[b2] = j;
                                            auto r = resolveR(machine, copy);
                                            if (r < result) {
                                                result = r;
                                            }
                                        }
                                    }

                                    return result;
                                    break;
                                }
                            }
                        }
                    }

                    break;
                }
            }

            if (solutions.size() == machine.buttonsLen) {

                if (machine.verify(solutions)) {
                    int r = 0;
                    for (auto &kv : solutions) {
                        r += kv.second;
                    }
                    return r;
                } else {
                    return 100000;
                }
                break;
            }
        }

        return result;
    }

    int resolve3(const Machine &machine) {
        int result{0};

        print(machine.matrix);

        Solutions s;
        result = resolveR(machine, s);

        log << "result = " << result << endl;

        return result;
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
            // print(machine.matrix);
            machine.simplify();
            result += resolve3(machine);
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

        // too low : 20618
        // too high : 20848
    }
}

int main() {
    // process("sample.txt");
    // process("sample2.txt");
    process("input.txt");
    return 0;
}
