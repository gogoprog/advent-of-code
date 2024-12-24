#include "../../common_fast.h"

enum Op { AND, OR, XOR, ADD };

struct Context {

    using States = Map<StringView, bool>;

    States wireStates;

    struct Connection {
        StringView a;
        StringView b;
        Set<StringView> terms;
        StringView target;
        int op;

        void print() {
            log << a << " " << op << " " << b << " -> " << target << endl;
        }
    };

    StringViews targets;
    StringViews inputXs;
    StringViews inputYs;

    using Connections = Vector<Connection>;

    Connections connections;
    Map<StringView, int> targetToConnectionIndex;

    void parse(auto content) {

        auto blocks = content | rv::split_sv("\n\n");

        for (auto line : blocks | rv::get0 | rv::split_sv('\n')) {
            auto parts = line | rv::split_sv(": ");

            wireStates[parts | rv::get0] = (parts | rv::get1) == "1";
        }

        for (auto line : blocks | rv::get1 | rv::split_sv('\n')) {
            auto parts = line | rv::split_sv(' ');

            connections.push_back({});
            auto &connection = connections.back();

            connection.a = parts | rv::get0;
            connection.b = parts | rv::get2;
            connection.target = parts | rv::get4;

            connection.terms.insert(connection.a);
            connection.terms.insert(connection.b);

            targets.push_back(connection.target);

            targetToConnectionIndex[connection.target] = connections.size() - 1;

            auto op = parts | rv::get1;

            if (op == "AND") {
                connection.op = AND;
            } else if (op == "OR") {
                connection.op = OR;
            } else if (op == "XOR") {
                connection.op = XOR;
            } else {
                log << "Error!" << endl;
            }
        }

        std::sort(targets.begin(), targets.end());

        for (auto kv : wireStates) {
            if (kv.first.starts_with('x')) {
                inputXs.push_back(kv.first);
            }

            if (kv.first.starts_with('y')) {
                inputYs.push_back(kv.first);
            }
        }
    }

    static bool apply(auto a, auto b, auto op) {
        switch (op) {
            case AND:
                return a & b;
            case OR:
                return a | b;
            case XOR:
                return a ^ b;
            default:
                break;
        }

        log << "Error!!" << endl;

        return 0;
    }

    static Uint64 apply2(auto a, auto b, auto op) {
        switch (op) {
            case AND:
                return a & b;
            case OR:
                return a | b;
            case XOR:
                return a ^ b;
            case ADD:
                return a + b;
        }

        log << "Error!!" << endl;

        return 0;
    }

    static Uint64 solve(const States &states, char c) {
        std::bitset<64> bs;
        auto index = 0;

        for (auto &kv : states) {
            if (kv.first.starts_with(c)) {
                bs.set(index, kv.second);
                index++;
            }
        }

        return bs.to_ullong();
    }

    void process(States &states, const Connections &connections) {
        bool all_done = false;

        while (!all_done) {
            all_done = true;

            for (auto &conn : connections) {

                if (states.find(conn.target) == states.end()) {
                    if (states.find(conn.a) == states.end() || states.find(conn.b) == states.end()) {
                        all_done = false;
                        /* log << conn.target << " not ready" << endl; */
                        continue;
                    }

                    states[conn.target] = apply(states[conn.a], states[conn.b], conn.op);
                    /* log << conn.target << " = " << states[conn.target] << endl; */
                }
            }
        }
    }

    void fill(States &states, Uint64 x, Uint64 y) {
        std::bitset<64> bs_x{x};
        std::bitset<64> bs_y{y};

        int x_index = 0;
        int y_index = 0;

        for (auto &kv : states) {
            if (kv.first.starts_with('x')) {
                kv.second = bs_x[x_index];
                x_index++;
            }

            if (kv.first.starts_with('y')) {
                kv.second = bs_y[y_index];
                y_index++;
            }
        }
    }

    void attempt(Uint64 x, Uint64 y, Uint64 z) {
        std::bitset<64> bs_x{x};
        std::bitset<64> bs_y{y};

        States states = wireStates;
        int x_index = 0;
        int y_index = 0;

        for (auto &kv : states) {
            if (kv.first.starts_with('x')) {
                kv.second = bs_x[x_index];
                x_index++;
            }

            if (kv.first.starts_with('y')) {
                kv.second = bs_y[y_index];
                y_index++;
            }
        }

        process(states, connections);

        auto result = solve(states, 'z');

        if (result != z) {
            log << "Invalid: " << x << ", " << y << ", " << z << " -> " << result << endl;
        }
    }

    void part1() {
        auto result{0_int64};

        auto states = wireStates;

        process(states, connections);

        result = solve(states, 'z');

        log << "Part1: " << result << endl;
    }

    void process(States &states, const Connections &connections, const Vector<Set<StringView>> &swaps) {
        bool all_done = false;

        Map<StringView, StringView> swaps_map;
        for (auto &swap : swaps) {
            swaps_map[*swap.begin()] = *swap.rbegin();
            swaps_map[*swap.rbegin()] = *swap.begin();
        }

        while (!all_done) {
            all_done = true;

            for (auto &conn : connections) {
                auto target = conn.target;

                if (swaps_map.find(target) != swaps_map.end()) {
                    target = swaps_map[target];
                }

                if (states.find(target) == states.end()) {
                    if (states.find(conn.a) == states.end() || states.find(conn.b) == states.end()) {
                        all_done = false;
                        /* log << conn.target << " not ready" << endl; */
                        continue;
                    }

                    states[target] = apply(states[conn.a], states[conn.b], conn.op);
                    /* log << conn.target << " = " << states[conn.target] << endl; */
                }
            }
        }
    }

    void reverse(auto name, auto expected) {

        Set<StringView> required;
        Set<int> requiredConnections;

        Queue<StringView> q;

        q.push(name);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (targetToConnectionIndex.find(node) != targetToConnectionIndex.end()) {

                auto index = targetToConnectionIndex[node];
                requiredConnections.insert(index);
                auto conn = connections[index];

                q.push(conn.a);
                q.push(conn.b);
            } else {
                required.insert(node);
            }
        }

        log << name << " requires " << requiredConnections << endl;

        log << requiredConnections.size() << endl;

        {
            struct Node {
                Vector<Set<StringView>> swaps;
            };

            Queue<Node> q;

            for (auto i : requiredConnections) {
                for (auto j : requiredConnections) {
                    if (i != j) {
                        auto conn_a = connections[i];
                        auto conn_b = connections[j];

                        q.push({{{conn_a.target, conn_b.target}}});
                    }
                }
            }

            while (!q.empty()) {
                const auto node = q.front();
                q.pop();

                if (node.swaps.size() == 1) {
                    States states = wireStates;
                    process(states, connections, node.swaps);
                    auto z = solve(states, 'z');

                    if (z == expected) {
                        log << "Found!" << endl;
                        log << node.swaps << endl;
                    }
                }
            }
        }
    }

    void reverse2(const auto &_states, auto _expected, Set<int> &_faulty) {

        auto states = _states;
        Map<StringView, int> expectedMap;
        std::bitset<64> expected(_expected);

        for (auto kv : states) {
            expectedMap[kv.first] = kv.second;
        }

        int index = 0;
        for (auto &target : targets) {

            if (target.starts_with('z')) {
                expectedMap[target] = expected[index];
                states[target] = expected[index];
                index++;
            }
        }

        bool all_done = false;

        Set<int> faulty;

        while (!all_done) {
            all_done = true;

            int i = 0;

            auto reverse_op = [&](int op, auto target, auto a, auto b) {
                switch (op) {
                    case XOR: {
                        auto r = states[target] ^ states[a];
                        states[b] = r;
                    } break;

                    case AND: {
                        if (states[target]) {
                            if (states[a]) {
                                states[b] = 1;
                            } else {
                                log << "Impossibru" << endl;
                                faulty.insert(i);
                            }

                        } else {
                            if (states[a]) {
                                states[b] = 0;
                            }
                        }
                    }

                    case OR: {
                        if (states[target]) {
                            if (!states[a]) {
                                states[b] = 1;
                            }
                        } else {
                            if (!states[a]) {
                                states[b] = 0;
                            } else {
                                log << "Impossibru" << endl;
                                faulty.insert(i);
                            }
                        }
                    }

                    break;
                }
            };

            for (auto &conn : connections) {

                bool target_is_expected = states.find(conn.target) != states.end();
                bool a_is_expected = states.find(conn.a) != states.end();
                bool b_is_expected = states.find(conn.b) != states.end();

                if (!target_is_expected) {
                    all_done = false;
                    if (a_is_expected && b_is_expected) {
                        auto r = apply(states[conn.a], states[conn.b], conn.op);
                        states[conn.target] = r;
                    }
                } else {

                    if (a_is_expected && b_is_expected) {

                        auto r = apply(states[conn.a], states[conn.b], conn.op);
                        if (r != states[conn.target]) {

                            /* states.erase(conn.a); */
                            /* states.erase(conn.b); */
                            faulty.insert(i);
                            log << "Wrong: ";
                            conn.print();
                            /* log << "Wrong: " << conn.target << " = " << r << ", expected " <<

                             * states[conn.target] */
                            /*     << endl; */
                        }
                    } else if (a_is_expected) {
                        reverse_op(conn.op, conn.target, conn.a, conn.b);
                        all_done = false;
                    } else {
                        reverse_op(conn.op, conn.target, conn.b, conn.a);
                        all_done = false;
                    }
                }

                ++i;
            }
        }

        /* if (ff.size() == 2) { */
        /* log << faulty << endl; */
        /* } */

        _faulty.insert(faulty.begin(), faulty.end());
    }

    void pp(const auto &_states, auto op, Set<StringView> &wrongs) {
        auto states = _states;
        auto x = solve(states, 'x');
        auto y = solve(states, 'y');
        log << "x = " << x << ", y = " << y << endl;

        auto expected = std::bitset<64>(apply2(x, y, op));

        process(states, connections);
        auto received = std::bitset<64>(solve(states, 'z'));

        if (expected != received) {
            log << "z should be " << expected.to_string() << endl;
            log << "but is      " << received.to_string() << endl;
        }

        int index = 0;
        for (auto &target : targets) {

            if (target.starts_with('z')) {
                if (expected[index] != received[index]) {
                    wrongs.insert(target);
                }

                index++;
            }
        }
    }

    int computeDepth(auto target) {
        for (auto &conn : connections) {
            if (conn.target == target) {
                int a = 1 + computeDepth(conn.a);
                int b = 1 + computeDepth(conn.b);
                return std::min(a, b);
            }
        }

        return 1;
    }

    bool dependsOn(auto target, auto other) {

        if (targetToConnectionIndex.find(target) == targetToConnectionIndex.end()) {
            return false;
        }

        auto conn = connections[targetToConnectionIndex[target]];

        if (conn.a == other) {
            return true;
        }

        if (conn.b == other) {
            return true;
        }

        if (dependsOn(conn.a, other)) {
            return true;
        }

        if (dependsOn(conn.b, other)) {
            return true;
        }

        return false;
    }

    void check(int _x, int _y, auto z) {

        StringView next;

        auto x = inputXs[_x];
        auto y = inputXs[_y];

        for (auto &conn : connections) {
            if (conn.terms.contains(x) && conn.terms.contains(y)) {
                if (conn.op == XOR) {
                    if (conn.target != z) {
                        log << "Problem" << endl;
                    }
                }

                if (conn.op == AND) {
                    next = conn.target;
                }
            }
        }

        if (!next.empty()) {
            StringView next2;
            for (auto &conn : connections) {
                if (conn.terms.contains(next) && conn.op == OR) {
                    next2 = conn.target;
                }
            }

            if (!next2.empty()) {
                /* check(_x+1, _y+1, */
            } else {
                log << "Problem3" << endl;
            }
        } else {
            log << "Problem2" << endl;
        }
    }

    const Connection *getConnectionWithTerm(StringView term) const {

        for (auto &conn : connections) {
            if (conn.terms.contains(term)) {
                return &conn;
            }
        }

        return nullptr;
    };

    void part2(auto count, auto op) {
        auto result{0_int64};

        /* check("x00", "y00", "z00"); */

        Set<StringView> wrongs;

        auto add_problem = [&](auto &conn) {
            if (!conn.terms.contains("x00")) {

                if (conn.target != "z45") {
                    /* log << "Prob : "; */
                    wrongs.insert(conn.target);
                }
            }
        };

        for (auto &conn : connections) {
            if (conn.target.starts_with('z')) {
                if (conn.op != XOR) {
                    add_problem(conn);
                }
            }

            if (conn.op == AND) {
                auto conn2 = getConnectionWithTerm(conn.target);

                if (conn2 && conn2->op != OR) {
                    add_problem(conn);
                }
            }

            if (conn.op == XOR) {
                auto conn2 = getConnectionWithTerm(conn.target);

                if (conn2 && (conn2->op != XOR && conn2->op != AND)) {

                    add_problem(conn);
                }

                if (!conn.target.starts_with('z')) {

                    for (auto t : conn.terms) {
                        if (!t.starts_with('x') && !t.starts_with('y')) {

                            add_problem(conn);
                        }
                    }
                }
            }
        }

        for (auto wrong : wrongs) {
            log << wrong << ",";
        }
        log << endl;
        return;
    }
};

void process(const char *filename, auto count, auto op) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    {
        Context context;
        context.parse(content);
        context.part1();
        context.part2(count, op);
    }
}

int main() {
    /* process("sample.txt"); */
    /* process("sample2.txt"); */
    /* process("sample3.txt", 2, AND); */
    process("input.txt", 4, ADD);
    return 0;
}
