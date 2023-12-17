#include "../../common_fast.h"

int computeHash(StringView str) {
    auto value = 0;

    for (auto c : str) {

        value += c;

        value *= 17;

        value %= 256;
    }

    return value;
}

struct Context {

    void part1(auto lines) {
        auto result{0};

        auto view = lines | rv::split_sv(',');

        for (auto word : view) {
            result += computeHash(word);
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {

        struct Lens {
            String label;
            int focal;
        };

        using Box = Vector<Lens>;

        Array<Box, 256> boxes;

        auto view = lines | rv::split_sv(',');

        for (auto word : view) {

            auto len = word.size();

            if (word[len - 2] == '=') {
                auto label = String(word.substr(0, len - 2));
                auto focal = word[len - 1] - '0';
                auto hash = computeHash(label);

                auto &box = boxes[hash];
                auto new_lens = Lens{label, focal};

                bool added = false;

                for (auto i = 0; i < box.size(); ++i) {
                    auto &lens = box[i];
                    if (lens.label == label) {
                        box.insert(box.begin() + i, new_lens);
                        box.erase(box.begin() + i + 1);
                        added = true;
                        break;
                    }
                }

                if (!added) {
                    box.push_back(new_lens);
                }
            } else if (word[len - 1] == '-') {
                auto label = word.substr(0, len - 1);
                auto hash = computeHash(label);

                auto &box = boxes[hash];

                for (auto it = box.begin(); it != box.end(); ++it) {
                    if (it->label == label) {
                        box.erase(it);
                        break;
                    }
                }
            } else {
                log << "Nop" << endl;
            }
        }

        auto result{0};

        for (auto b = 0; b < boxes.size(); ++b) {
            auto &box = boxes[b];

            for (auto l = 0; l < box.size(); ++l) {
                auto &lens = box[l];

                auto power = (1 + b) * (l + 1) * lens.focal;

                result += power;
            }
        }
        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileContent(filename);
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
