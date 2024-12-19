#include "../../common_fast.h"

struct Context {

    StringViews patterns;
    StringViews designs;

    void parse(auto lines) {
        rs::copy(lines | rv::get0 | rv::split_sv(", "), std::back_inserter(patterns));
        rs::copy(lines | rv::drop(2), std::back_inserter(designs));
    }

    bool attempt(const auto design, const int pos, Map<int, int> &visited) {
        if (visited[pos]) {
            visited[pos]++;
            return false;
        }

        if (pos == design.size()) {
            return true;
        }

        for (auto &pattern : patterns) {
            if (design.substr(pos, pattern.size()) == pattern) {
                if (attempt(design, pos + pattern.size(), visited)) {
                    return true;
                }
            }
        }

        visited[pos] = 1;

        return false;
    }

    void part1() {
        auto result{0_int64};

        for (auto design : designs) {
            Map<int, int> visited;
            if (attempt(design, 0, visited)) {
                result++;
            }
        }

        log << "Part1: " << result << endl;
    }

    Uint64 countAttempts(const auto design) {
        Uint64 result{0};

        struct Node {
            int pos;
        };

        Queue<Node> q;
        q.push({0});

        Map<int, bool> visited;
        Map<int, Map<int, int>> relinks;

        bool succeed = false;

        while (!q.empty()) {

            const auto node = q.front();
            q.pop();

            auto &v = visited[node.pos];

            if (v)
                continue;

            v = true;

            auto pos = node.pos;

            if (pos == design.size()) {
                succeed = true;
            } else {
                if (pos < design.size()) {
                    for (auto &pattern : patterns) {
                        if (design.substr(pos, pattern.size()) == pattern) {
                            auto npos = pos + pattern.size();
                            auto copy = node;
                            copy.pos = npos;
                            relinks[npos][pos]++;
                            q.push(copy);
                        }
                    }
                }
            }
        }

        if (!succeed) {
            return 0;
        }

        Map<int, Uint64> totals;

        totals[design.size()] = 1;

        for (int i = design.size(); i >= 0; i--) {
            auto count = totals[i];

            if (count) {
                for (auto kv : relinks[i]) {
                    totals[kv.first] += count * kv.second;
                }
            }
        }

        result = totals[0];

        return result;
    }

    void part2() {
        Uint64 result{0};

        for (auto design : designs) {
            result += countAttempts(design);
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
    process("sample.txt");
    process("input.txt");
    return 0;
}
