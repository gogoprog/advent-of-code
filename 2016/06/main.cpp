#include "../../common.h"

struct Context {
    Vector<Map<char, int>> counts;

    void init(const int s){
        counts.resize(s);
    }

    String word() const {
        String result;

        for (auto &map : counts) {

            auto it = std::max_element(map.begin(), map.end(), [](auto &a, auto &b) { return a.second < b.second; });

            result += (*it).first;
        }

        return result;
    }

    String word2() const {
        String result;

        for (auto &map : counts) {

            auto it = std::min_element(map.begin(), map.end(), [](auto &a, auto &b) { return a.second < b.second; });

            result += (*it).first;
        }

        return result;
    }

    void process(const String line) {
        for (int i{0}; i < line.length(); ++i) {
            char c = line[i];

            auto &map = counts[i];

            map[c] = map[c] + 1;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;

    ctx.init(lines[0].length());

    for (auto line : lines) {
        ctx.process(line);
    }

    log << "part1: " << ctx.word() << endl;
    log << "part2: " << ctx.word2() << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
