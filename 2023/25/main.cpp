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

    int getId(StringView name) {

        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                return i;
            }
        }

        names.push_back(String(name));

        return names.size() - 1;
    }

    void part1(auto lines) {
        Map<int, Set<int>> connections;
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
            log << kv.first << " -> " << kv.second.size() << " | " << kv.second << "\n";
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
            size_t notcommon;

            bool operator<(const Entry &other) const {
                return other.notcommon < notcommon;
            }
        };

        Vector<Entry> entries;

        for (auto &kv : connections) {
            for (auto &kv2 : connections) {

                if (kv.first != kv2.first) {
                    if (connections[kv.first].contains(kv2.first)) {

                        if (!possibilities.contains({kv.first, kv2.first})) {

                            auto r = count_common(kv.second, kv2.second);
                            auto r2 = kv.second.size() + kv2.second.size() - r;
                            /* log << kv.first << " & " << kv2.first << " = " << r << " | " << r2 << "\n"; */
                            /* commons[{kv.first, kv2.first}] = r; */

                            entries.push_back({{kv.first, kv2.first}, r, r2});
                            if (r == 0) {
                                /* entries.push_back({{kv.first, kv2.first}, r, r2}); */
                                /* log << kv.first << kv.second.size() << " -> " << kv2.first << kv2.second.size() <<
                                 * "\n";
                                 */
                                /* log << kv.first << " & " << kv2.first << " = " << r << " | " << r2 << "\n"; */
                            }
                            possibilities.insert({kv.first, kv2.first});
                        }
                    }
                }
            }
        }

        std::sort(entries.begin(), entries.end());

        entries.resize(std::min<int>(entries.size(), 200));

        for (auto entry : entries) {
            log << entry.pair << " | " << entry.common << " | " << entry.notcommon << "\n";
        }

        auto len = entries.size();

        Set<Set<int>> done;

        for (int i = 0; i < len; ++i) {
            for (int j = 0; j < len; ++j) {
                for (int k = 0; k < len; ++k) {
                    if (i != k && i != j) {

                        if (!done.contains({i, j, k})) {

                            done.insert({i, j, k});

                            auto &a = entries[i].pair;
                            auto &b = entries[j].pair;
                            auto &c = entries[k].pair;

                            disconnections.clear();

                            disconnect(*a.begin(), *std::next(a.begin()));
                            disconnect(*b.begin(), *std::next(b.begin()));
                            disconnect(*c.begin(), *std::next(c.begin()));

                            int r;
                            auto total = count_groups(r);

                            if (total == 2) {
                                log << i << " & " << j << " & " << k << endl;
                                log << "yep : " << a << b << c << "\n";
                                result = r;
                                log << "Part1: " << result << endl;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    void part2(auto lines) {
        auto result{0};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
