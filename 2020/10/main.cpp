#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Vector<int> values;

    for (auto &line : lines) {
        values.push_back(std::stoi(line));
    }

    std::sort(values.begin(), values.end());

    auto builtinjolts = *(std::max_element(values.begin(), values.end())) + 3;

    values.push_back(builtinjolts);

    Map<int, int> joltdiffs;

    int current{0};

    for (auto v : values) {
        joltdiffs[v - current]++;
        current = v;
    }

    log << "Part1 : " << joltdiffs[1] * joltdiffs[3] << endl;

    {
        auto compute = [](const int input) {
            ull n = input - 1;
            return n * (n-1)/2 + 1;
        };

        ull count{1};
        int current{0};
        int consec{1};

        for (auto v : values) {
            if (v - current < 3) {
                consec++;
            } else {
                if (consec > 2) {
                    count *= compute(consec);
                }
                consec = 1;
            }
            current = v;
        }

        log << "Part2 : " << count << endl;
    }
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
