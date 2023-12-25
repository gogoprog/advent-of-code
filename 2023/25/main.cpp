#include "../../common_fast.h"

struct Groups {
    Vector<Set<int>> values;

    Set<int> &findGroup(int name) {
        for (auto &group : values) {
            if (group.contains(name)) {
                return group;
            }
        }

        values.push_back({name});

        return values.back();
    };

    bool hasGroup(int name) const {
        for (auto &group : values) {

            if (group.contains(name)) {
                return true;
            }
        }

        return false;
    }
};

struct Context {

    Vector<String> names;
    Map<int, Set<int>> connections;

    int getId(StringView name) {

        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                return i;
            }
        }

        names.push_back(String(name));

        return names.size() - 1;
    }

    int compute(int start, int end) {

        int result{0};

        struct Node {
            int current;
            int steps{0};
        };

        Queue<Node> q;

        q.push({start});

        Map<int, int> visited;

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            auto &v = visited[node.current];
            if (v == 0 || v > node.steps) {
                v = node.steps;
            } else {
                continue;
            }

            if (node.current == end) {
                result = node.steps;
                continue;
            }

            for (auto connection : connections[node.current]) {
                if (!(node.current == start && connection == end)) {
                    auto copy = node;
                    copy.current = connection;
                    copy.steps++;
                    q.push(copy);
                }
            }
        }

        return result;
    }

    void part1(auto lines) {
        auto result{0};

        for (auto line : lines) {
            auto name = line | rv::split_sv(':') | rv::get0;
            auto list = line | rv::split_sv(':') | rv::get1 | rv::split_sv(' ') | rv::filter_empty;

            /* log << name << " :\n"; */
            for (auto other : list) {
                /* log << "\t" << other << "\n"; */
                connections[getId(name)].insert(getId(other));
                connections[getId(other)].insert(getId(name));
            }
        }

        Set<Pair<int, int>> disconnections;

        auto disconnect = [&](int a, int b) {
            disconnections.insert({a, b});
            disconnections.insert({b, a});
        };

        auto count_groups = [&](int &result) {
            static Groups groups;
            groups.values.resize(0);

            groups.values.push_back({connections.begin()->first});

            bool working = true;
            while (working) {
                working = false;

                bool found = true;
                while (found) {
                    found = false;

                    for (auto &kv : connections) {
                        auto that = kv.first;

                        if (groups.hasGroup(that)) {

                            auto &that_group = groups.findGroup(that);

                            for (auto other : kv.second) {
                                /* log << disconnections.size() << "\n"; */
                                /* log << that << " -> " << other << "\n"; */
                                if (!disconnections.contains({that, other})) {

                                    if (!that_group.contains(other)) {
                                        that_group.insert(other);
                                        found = true;
                                    }
                                } else {
                                    /* log << "disco!" << endl; */
                                }
                            }
                        }
                    }
                }

                for (auto &kv : connections) {
                    auto that = kv.first;

                    if (!groups.hasGroup(that)) {
                        groups.values.push_back({that});
                        working = true;
                        break;
                    }
                }
            }

            if (groups.values.size() == 2) {
                result = groups.values[0].size() * groups.values[1].size();
            }
            /* log << groups.values << endl; */

            return groups.values.size();
        };

        for (auto &kv : connections) {
            /* log << kv.first << " -> " << kv.second.size() << " | " << kv.second << "\n"; */
        }

        /* log << count_groups() << endl; */
        /* disconnect("hfx", "pzl"); */
        /* disconnect("bvb", "cmg"); */
        /* disconnect("nvd", "jqt"); */
        /* log << count_groups() << endl; */

        auto count_common = [](auto &s1, auto &s2) {
            Set<int> intersect;
            std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                                  std::inserter(intersect, intersect.begin()));
            return intersect.size();
        };

        Set<Set<int>> possibilities;
        Map<Set<int>, int> commons;

        struct Entry {
            Set<int> pair;
            size_t common;
            int value;

            int getFirst() const {
                return *pair.begin();
            }

            int getSecond() const {
                return *(pair.rbegin());
            }

            bool operator<(const Entry &other) const {
                return other.value < value;
            }
        };

        Vector<Entry> entries;

        for (auto &kv : connections) {
            for (auto &kv2 : connections) {

                if (kv.first != kv2.first) {
                    if (connections[kv.first].contains(kv2.first)) {

                        if (!possibilities.contains({kv.first, kv2.first})) {

                            auto r = count_common(kv.second, kv2.second);

                            auto r2 = compute(kv.first, kv2.first);

                            entries.push_back({{kv.first, kv2.first}, r, r2});

                            possibilities.insert({kv.first, kv2.first});
                        }
                    }
                }
            }
        }

        std::sort(entries.begin(), entries.end());

        auto &a = entries[0].pair;
        auto &b = entries[1].pair;
        auto &c = entries[2].pair;

        disconnections.clear();

        disconnect(*a.begin(), *std::next(a.begin()));
        disconnect(*b.begin(), *std::next(b.begin()));
        disconnect(*c.begin(), *std::next(c.begin()));

        int r;
        auto total = count_groups(r);

        if (total == 2) {
            /* log << "yep : " << a << b << c << "\n"; */
            result = r;
            log << "Part1: " << result << endl;
            return;
        }
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.part1(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
