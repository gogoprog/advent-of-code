#include "../../common.h"

static const Vector<Point> &deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

struct Context {
    Strings lines;

    bool isValid(int x, int y) const {
        return y < lines.size() && x < lines[0].size();
    }

    bool isSymbol(int x, int y) const {

        if (isValid(x, y)) {
            char c = lines[y][x];

            if (!std::isdigit(c)) {
                return c != '.';
            }
        }

        return false;
    }

    void parse(const Strings &lines) {
        this->lines = lines;
    }

    void part1() {
        auto result{0};

        String current = "";
        bool adjacent_symbol{false};

        Map<Point, Set<int>> starMap;

        Vector<Point> currentStars;

        auto check = [&]() {
            if (current != "") {

                if (adjacent_symbol) {
                    auto value = std::stoi(current);
                    result += value;

                    for (auto s : currentStars) {
                        starMap[s].insert(value);
                    }
                }

                current = "";
                adjacent_symbol = false;
                currentStars.clear();
            }
        };

        for (int y = 0; y < lines.size(); ++y) {

            for (int x = 0; x < lines[0].size(); ++x) {

                char c = lines[y][x];

                if (std::isdigit(c)) {
                    current += c;

                    for (auto delta : deltas) {

                        auto nx = x + delta.x;
                        auto ny = y + delta.y;

                        if (isSymbol(nx, ny)) {

                            adjacent_symbol = true;

                            if (lines[ny][nx] == '*') {
                                currentStars.push_back({nx, ny});
                            }
                            /* break; */
                        }
                    }

                } else {
                    check();
                }
            }

            check();
        }

        auto result2 = 0;

        for (auto kv : starMap) {
            if (kv.second.size() == 2) {
                auto begin = kv.second.begin();
                result2 += *begin * *(++begin);
            }
        }

        log << "Part1: " << result << endl;
        log << "Part2: " << result2 << endl;
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
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
