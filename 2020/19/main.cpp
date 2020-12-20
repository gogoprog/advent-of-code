#include "../../common.h"

using Result = Pair<bool, int>;

template <int max> struct FixedArray {
    int values[max];
    int size{0};

    int &operator[](const int index) {
        return values[index];
    }

    const int &operator[](const int index) const {
        return values[index];
    }

    void shift(const int from, const int offset) {
        size += offset;
        for (int s = size - offset - 1; s >= from; --s) {
            values[s + offset] = values[s];
        }
    }

    void push(const int value) {
        values[size++] = value;
    }

    bool operator<(const FixedArray &other) const {
        if (size == other.size) {
            for (int i{0}; i < size; ++i) {
                if (values[i] != other.values[i]) {
                    return values[i] < other.values[i];
                }
            }
        }

        return size < other.size;
    }
};

template <int T> std::ostream &operator<<(std::ostream &out, const FixedArray<T> &v) {
    out << '[';
    bool first{true};
    for (int i{0}; i < v.size; ++i) {
        if (!first)
            out << ", ";
        out << v.values[i];
        first = false;
    }
    out << "]";
    return out;
}

struct Rule {
    char c{0};
    Vector<Vector<int>> dependingRules;
};

struct System {
    Vector<Rule> rules;

    void parseRule(const String line) {
        InputStringStream iss{line};
        String str;
        char tmp;
        int rn;

        iss >> rn >> tmp;

        if (rn >= rules.size()) {
            rules.resize(rn + 1);
        }

        auto &rule = rules[rn];
        rule = {};
        rule.dependingRules.push_back({});

        auto *deps = &rule.dependingRules.back();

        while (!iss.eof()) {
            iss >> str;

            if (str == "|") {
                rule.dependingRules.push_back({});
                deps = &rule.dependingRules.back();
            } else {
                if (str[0] == '"') {
                    rule.c = str[1];
                } else {
                    deps->push_back(std::stoi(str));
                }
            }
        }
    }

    Result test(const String line, const int ruleIndex, const int offset, const int skips = 0) const {
        const auto &rule = rules[ruleIndex];

        if (offset >= line.length()) {
            return {false, offset + 1};
        }

        if (rule.c) {
            return {line[offset] == rule.c, offset + 1};
        } else {

            for (auto &deps : rule.dependingRules) {
                int o{offset};
                auto pass{true};
                for (auto &dep : deps) {
                    auto res = test(line, dep, o);

                    if (!res.first) {
                        pass = false;
                        break;
                    }

                    o = res.second;
                }

                if (pass) {
                    return {true, o};
                }
            }

            return {false, offset};
        }
    }

    static constexpr int AS = 100;
    bool test2(const String line) const {
        struct BfsNode {
            /* Vector<int> rules; */
            /* Array<int, 100> rules; */
            FixedArray<AS> rules;

            bool operator<(const BfsNode &other) const {
                /* return rules.size() < other.rules.size(); */
                return rules.size < other.rules.size;
            }
        };

        PriorityQueue<BfsNode> q;
        /* Map<BfsNode, bool> visited; */
        Map<FixedArray<AS>, bool> visited;

        FixedArray<AS> start;
        start.push(0);
        q.push({start});

        /* q.push({{0}}); */

        while (!q.empty()) {

            const auto node = q.top();
            q.pop();

            if (node.rules.size > line.length()) {
                continue;
            }

            /* if (visited[node.rules]) */
            /*     continue; */

            /* visited[node.rules] = true; */

            {
                bool skip{false};
                bool allgood{true};
                for (int i{0}; i < node.rules.size; ++i) {
                    auto &rule = rules[node.rules[i]];
                    if (rule.c) {
                        if (rule.c != line[i]) {
                            allgood = false;
                            skip = true;
                            break;
                        }
                    } else {
                        allgood = false;
                        break;
                    }
                }

                if (skip) {
                    continue;
                }

                if (allgood) {
                    if (node.rules.size == line.length()) {
                        /* log << "found!" << endl; */
                        return true;
                    }
                }
            }

            for (int i{0}; i < node.rules.size; ++i) {
                auto &rule = rules[node.rules[i]];

                for (auto &deps : rule.dependingRules) {

                    if (deps.size() > 0) {

                        auto copy = node;
                        int decal = deps.size() - 1;

                        copy.rules.shift(i, decal);

                        for (int s{0}; s < deps.size(); ++s) {
                            copy.rules[i + s] = deps[s];
                        }
                        q.push(copy);
                    }
                }

                if(!rule.c) {
                    break;
                }
            }
        }

        return false;
    }

    bool test(const String line) const {
        auto res = test(line, 0, 0);
        return res.first && res.second == line.length();
    }
};

void process(const String filename, const bool part2 = false) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    System system{};

    bool itIsRules{true};

    auto r{0};

    for (auto &line : lines) {
        if (line == "") {
            itIsRules = false;

            if (part2) {
                system.parseRule("8: 42 | 42 8");
                system.parseRule("11: 42 31 | 42 11 31");
            }
        } else {
            if (itIsRules) {
                system.parseRule(line);
            } else {
                auto test = system.test2(line);

                /* log << line << " = " << test << endl; */

                if (test) {
                    r++;
                }
            }
        }
    }

    if (part2) {
        log << "Part2: " << r << endl;
    } else {
        log << "Part1: " << r << endl;
    }
}

int main() {
    /* process("sample.txt"); */
    process("sample2.txt");
    process("sample2.txt", true);
    /* process("sample3.txt"); */
    process("input.txt");
    process("input.txt", true);
    return 0;
}
