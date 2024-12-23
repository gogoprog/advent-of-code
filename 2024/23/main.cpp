#include "../../common_fast.h"

struct Context {

    Map<String, Set<String>> connections;
    Set<String> names;

    void parse(auto lines) {

        for (auto line : lines) {
            auto a = line | rv::split_sv('-') | rv::get0;
            auto b = line | rv::split_sv('-') | rv::get1;

            auto stra = String(a);
            auto strb = String(b);

            connections[stra].insert(strb);
            connections[strb].insert(stra);
            names.insert(stra);
            names.insert(strb);
        }
    }

    void part1() {
        auto result{0_int64};

        Set<Set<String>> inters;

        for (auto &name1 : names) {
            for (auto &name2 : names) {
                if (connections[name1].contains(name2)) {
                    for (auto &name3 : names) {
                        if (name1.starts_with('t') || name2.starts_with('t') || name3.starts_with('t')) {
                            if (name1 != name2 && name1 != name3) {
                                if (connections[name1].contains(name3)) {
                                    if (connections[name2].contains(name3)) {
                                        inters.insert({name1, name2, name3});
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        result = inters.size();

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        Queue<String> q{names.begin(), names.end()};

        using Group = Set<String>;
        Vector<Group> groups;

        for (auto &name : names) {
            groups.push_back({name});
        }

        while (!q.empty()) {
            auto name = q.front();
            q.pop();

            auto &conns = connections[name];

            bool found_group = false;

            for (auto &group : groups) {
                bool all_match = true;

                for (auto &entry : group) {
                    if (!conns.contains(entry)) {
                        all_match = false;
                    }
                }

                if (all_match) {
                    group.insert(name);
                }
            }
        }

        int best = 0;
        Group *best_group;
        for (auto &group : groups) {
            if (group.size() > best) {
                best = group.size();
                best_group = &group;
            }
        }

        log << "Part2: ";

        for (auto &entry : *best_group) {
            log << entry << ',';
        }

        log << endl;
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
    process("sample.txt");
    process("input.txt");
    return 0;
}
