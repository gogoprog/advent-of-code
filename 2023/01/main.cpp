#include "../../common.h"

struct Context {

    Vector<Vector<int>> iLines;
    Strings lines;

    void parse(const Strings &lines) {
        this->lines = lines;

        for (auto &line : lines) {

            auto &iline = iLines.emplace_back();

            for (char c : line) {

                if (std::isdigit(c)) {
                    iline.push_back(c - '0');
                }
            }
        }
    }

    void part1() {
        auto result{0};

        for (auto iline : iLines) {

            result += iline.back();
            result += iline.front() * 10;
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        Strings spells{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
        Strings digits{"1", "2", "3", "4", "5", "6", "7", "8", "9"};
        auto result{0};

        for (auto &line : lines) {

            int val = 0;
            int bestpos = 1000;
            int val2 = 0;
            int bestpos2 = -1;

            for (int i = 0; i < 9; ++i) {

                {
                    int it = line.find(spells[i]);
                    int it2 = line.find(digits[i]);

                    if (it != String::npos) {
                        if (it < bestpos) {
                            bestpos = it;
                            val = i+1;
                        }
                    }

                    if (it2 != String::npos) {
                        if (it2 < bestpos) {
                            bestpos = it2;
                            val = i+1;
                        }
                    }
                }
                {
                    int it = line.rfind(spells[i]);
                    int it2 = line.rfind(digits[i]);

                    if (it != String::npos) {
                        if (it > bestpos2) {
                            bestpos2 = it;
                            val2 = i+1;
                        }
                    }

                    if (it2 != String::npos) {
                        if (it2 > bestpos2) {
                            bestpos2 = it2;
                            val2 = i+1;
                        }
                    }
                }
            }

            result += val * 10 + val2;
        }

        log << "Part2: " << result << endl;
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
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
