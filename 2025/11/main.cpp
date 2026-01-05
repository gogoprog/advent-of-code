#include "../../common_fast.h"

struct Context {

    Map<StringView, Vector<StringView>> connections;
    Map<StringView, Vector<StringView>> invconnections;
    Set<StringView> all;

    Map<Pair<StringView, StringView>, bool> reachables;

    void parse(auto lines) {
        for (auto line : lines) {
            auto parts = line | rv::split_sv(':');
            auto main = parts | rv::get0;
            auto list = parts | rv::get1 | rv::split_sv(' ') | rv::filter_empty;

            for (auto entry : list) {
                connections[main].push_back(entry);
                invconnections[entry].push_back(main);
                all.insert(main);
                all.insert(entry);
            }
        }
    }

    int solve(const StringView start, const StringView end) {
        int result{0};

        struct Node {
            Vector<StringView> history;
        };

        Queue<Node> q;
        q.push({{start}});

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            auto current = node.history.back();

            if (current == end) {
                result++;
            } else {

                for (auto link : connections[node.history.back()]) {
                    auto copy = node;
                    // copy.history.resize(0);
                    copy.history.push_back(link);
                    q.push(copy);
                }
            }
        }

        return result;
    }

    Uint64 solve2(const StringView start, const StringView end, const StringView exclude = "") {
        Uint64 result{0};

        struct Node {
            StringView current;
            Uint64 total{1};
        };

        Queue<Node> q;
        q.push({start});

        Map<StringView, Uint64> visited;

        Set<StringView> todo;

        while (!q.empty()) {

            while (!q.empty()) {
                const auto node = q.front();
                q.pop();

                auto current = node.current;

                if (current == exclude)
                    continue;

                if (todo.find(current) != todo.end()) {
                    visited[current] += node.total;
                    continue;
                }

                if (current == end) {
                    result += node.total;
                    log << "found!" << endl;
                    log << node.total << endl;
                } else {
                    for (auto link : connections[current]) {
                        auto copy = node;
                        copy.current = link;
                        q.push(copy);
                        todo.insert(link);
                    }
                }
            }

            for (auto name : todo) {
                q.push({name, visited[name]});
            }

            visited.clear();
            todo.clear();
        }

        return result;
    }

    void part1() {
        auto result{0_int64};

        result = solve("you", "out");

        log << "Part1: " << result << endl;
    }

    void part2() {
        Uint64 result{0};

        auto a = solve2("svr", "dac", "fft");
        auto b = solve2("svr", "fft", "dac");
        auto c = solve2("dac", "fft");
        auto d = solve2("fft", "dac");
        auto e = solve2("dac", "out", "fft");
        auto f = solve2("fft", "out", "dac");

        result = a * c * f + b * d * e;

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

        // too high :  387421406407565
        // too high : 1553008854826960
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
