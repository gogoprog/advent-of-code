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

        bool result = false;
        for (auto &pattern : patterns) {
            if (design.substr(pos, pattern.size()) == pattern) {
                if (attempt(design, pos + pattern.size(), visited)) {
                    /* return true; */
                    result = true;
                }
            }
        }

        visited[pos] = 1;

        return result;
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

    void part2() {
        auto result{0_int64};

        for (auto design : designs) {
            Map<int, int> visited;
            if (attempt(design, 0, visited)) {

                Int64 value = 1;

                for (int i = 0; i <= design.size(); ++i) {
                    value *= visited[i] ? visited[i] : 1;
                }

                result += value;

                /* log << value << endl; */
            }
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
