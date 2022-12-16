#include "../../common.h"

struct Valve {
    String name;
    int rate;
    Strings connections;

    int compute(const int time) const {
        return rate * time;
    }
};

struct Context {
    Map<String, Valve> valves;
    Vector<Valve> activeValves;
    Map<Pair<int, int>, int> costs;

    void parse(const Strings &lines) {
        String str;

        for (auto &line : lines) {
            InputStringStream iss{line};

            Valve valve;
            iss >> str >> valve.name >> str >> str;
            iss.ignore(6);
            iss >> valve.rate;
            iss >> str >> str >> str >> str >> str;
            std::getline(iss, str);

            valve.connections = splitString(str, ',');

            for (auto &name : valve.connections) {
                name.erase(remove(name.begin(), name.end(), ' '), name.end());
            }

            /* log << valve.name << " " << valve.rate << " " << valve.connections << endl; */

            valves[valve.name] = valve;
        }

        activeValves.push_back(valves["AA"]);

        for (auto &kv : valves) {
            if (kv.second.rate) {
                activeValves.push_back(kv.second);
            }
        }

        for (int v1{0}; v1 < activeValves.size(); ++v1) {
            for (int v2{0}; v2 < activeValves.size(); ++v2) {
                if (v1 != v2) {
                    auto cost = computeCost(activeValves[v1].name, activeValves[v2].name);
                    costs[{v1, v2}] = cost;
                }
            }
        }
    }

    int getActiveValveIndex(const String &name) const {
        for (int v1{0}; v1 < activeValves.size(); ++v1) {
            if (activeValves[v1].name == name)
                return v1;
        }
        return -1;
    }

    int computeCost(const String from, const String to) {
        struct Node {
            String position;
            int time{0};

            bool operator<(const Node &other) const {
                /* if (time != other.time) { */
                return time < other.time;
                /* } */
            }
        };

        PriorityQueue<Node> q;
        q.push({from});

        int best = 100000;

        Map<String, int> visited;

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (visited[node.position] && visited[node.position] < node.time) {
                continue;
            } else {
                visited[node.position] = node.time;
            }

            if (node.time > best) {
                continue;
            }

            if (node.position == to) {
                if (node.time < best) {
                    best = node.time;
                }
                continue;
            }

            const auto &current_valve = valves[node.position];
            for (auto &name : current_valve.connections) {
                auto copy = node;
                copy.position = name;
                copy.time++;
                q.push(copy);
            }
        }

        return best;
    }

    struct Node {
        int position;
        int time{0};
        Array<int, 16> openedTimes;
        int potential{0};
        /* Strings history; */

        Node(const int pos) {
            position = pos;
            for (auto &v : openedTimes) {
                v = -1;
            }
        }

        bool operator<(const Node &other) const {
            return time < other.time;
        }
    };

    int compute(const Array<int, 16> &openedTimes, const int time) {
        int result{0};

        for (int i = 1; i < activeValves.size(); ++i) {
            if (openedTimes[i] != -1) {
                result += (time - openedTimes[i]) * activeValves[i].rate;
            }
        }

        return result;
    }

    int computePotential(const Array<int, 16> &openedTimes, const int time, const int max_time) {
        int result = compute(openedTimes, max_time);

        int rest = max_time - time;

        for (int i = 1; i < activeValves.size(); ++i) {
            if (openedTimes[i] == -1) {
                result += (rest)*activeValves[i].rate;
            }
        }

        return result;
    }

    Node computeTotalPressure(const int max_time) {
        Node best_node{-1};
        PriorityQueue<Node> q;
        q.push({0});
        int best{0};

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (node.time > max_time) {
                continue;
            }

            if (node.time == max_time) {
                continue;
            }

            if (node.potential < best) {
                continue;
            }

            {
                auto total = compute(node.openedTimes, max_time);

                if (total > best) {
                    best_node = node;
                    best = total;
                }
            }

            {
                for (int v{1}; v < activeValves.size(); ++v) {
                    const auto &valve = activeValves[v];
                    if (node.openedTimes[v] == -1) {
                        auto copy = node;
                        copy.position = v;
                        copy.time += costs[{node.position, v}];
                        copy.time++;
                        copy.openedTimes[v] = copy.time;
                        copy.potential = computePotential(copy.openedTimes, copy.time, max_time);
                        q.push(copy);
                    }
                }
            }
        }

        return best_node;
    }

    void part1() {
        auto best = computeTotalPressure(30);
        auto result = compute(best.openedTimes, 30);
        log << "Part1: " << result << endl;
        /* log << best.history << endl; */
    }

    void part2() {
        const auto max_time = 26;
        struct Node {
            int positions[2];
            int time[2];
            Array<int, 16> openedTimes;
            int potential{0};

            Node(const int pos) {
                positions[0] = pos;
                positions[1] = pos;
                time[0] = 0;
                time[1] = 0;
                for (auto &v : openedTimes) {
                    v = -1;
                }
            }

            bool operator<(const Node &other) const {
                return time[0] + time[1] < other.time[0] + other.time[1];
            }
        };

        Node best_node{-1};
        PriorityQueue<Node> q;
        q.push({0});

        int best{0};

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (node.time[0] > max_time || node.time[1] > max_time) {
                continue;
            }

            if (node.potential < best) {
                continue;
            }

            /* log << node.time[0] << " & " << node.time[1] << endl; */

            {
                auto total = compute(node.openedTimes, max_time);

                if (total > best) {
                    best_node = node;
                    best = total;
                }
            }

            for (int i{0}; i < 2; ++i) {
                for (int v{1}; v < activeValves.size(); ++v) {
                    const auto &valve = activeValves[v];
                    if (node.openedTimes[v] == -1) {
                        auto copy = node;
                        copy.positions[i] = v;
                        copy.time[i] += costs[{node.positions[i], v}];
                        copy.time[i]++;
                        copy.openedTimes[v] = copy.time[i];
                        copy.potential = computePotential(copy.openedTimes, copy.time[i], max_time);
                        q.push(copy);
                    }
                }
            }
        }

        auto result = compute(best_node.openedTimes, max_time);

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
