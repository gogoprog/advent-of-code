#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    String openers = "([{<";
    String closers = ")]}>";

    Vector<int> scores = {3, 57, 1197, 25137};
    Vector<int> scores2 = {1, 2, 3, 4};
    Vector<ull> auto_complete_scores;

    for (auto &line : lines) {
        Vector<int> stack;
        auto corrupted = false;

        for (auto c : line) {

            auto opener_index = openers.find(c);

            if (opener_index != String::npos) {
                stack.push_back(opener_index);
            } else {
                auto closer_index = closers.find(c);

                if (closer_index != String::npos) {

                    auto last_index = stack.back();

                    if (last_index != closer_index) {

                        /* log << "Expected " << closers[last_index] << " but found " << c << endl; */

                        r += scores[closer_index];
                        corrupted = true;
                        break;

                    } else {
                        stack.pop_back();
                    }

                } else {
                    log << "What?" << c << endl;
                }
            }
        }

        if (!corrupted && !stack.empty()) {
            auto complete_score{0ull};
            for (auto opener_index : stack | rv::reverse) {
                complete_score *= 5;
                complete_score += scores2[opener_index];
            }
            auto_complete_scores.push_back(complete_score);
        }
    }

    log << "Part1: " << r << endl;

    {
        rs::sort(auto_complete_scores);

        auto r = auto_complete_scores[auto_complete_scores.size() / 2];
        log << "Part2: " << r << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
