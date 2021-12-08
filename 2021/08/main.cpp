#include "../../common.h"
#include <algorithm>

struct Digit {
    int value;
    String original;
    Vector<String> matches;

    bool accept(const String solution) {
        for (auto &match : matches) {
            if (std::all_of(match.begin(), match.end(), [&](auto c) {
                    auto remap = solution[c - 'a'];

                    return original.find(remap) != String::npos;
                })) {
                return true;
            }
        }

        return false;
    }

    bool match(const String solution, const String value) {
        if (value.size() == original.size()) {
            if (std::all_of(original.begin(), original.end(), [&](auto c) {
                    auto remap = solution[c - 'a'];

                    return value.find(remap) != String::npos;
                })) {
                return true;
            }
        }
        return false;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;
    auto r2 = 0;

    Vector<Digit> digits;

    digits.emplace_back(0, "abcefg");
    digits.emplace_back(1, "cf");
    digits.emplace_back(2, "acdeg");
    digits.emplace_back(3, "acdfg");
    digits.emplace_back(4, "bcdf");
    digits.emplace_back(5, "abdfg");
    digits.emplace_back(6, "abdefg");
    digits.emplace_back(7, "acf");
    digits.emplace_back(8, "abcdefg");
    digits.emplace_back(9, "abcdfg");

    for (auto &line : lines) {

        for(auto & digit : digits)
        {
            digit.matches.resize(0);
        }

        String solution = "abcdefg";
        String reverse_solution;

        auto [left, right] = splitNString<String, 2>(line, '|');

        auto left_words = splitString(left, ' ');
        auto right_words = splitString(right, ' ');
        auto all_words = splitString(line, ' ');

        for (auto &word : right_words) {
            auto len = word.size();

            if (len == 2 || len == 4 || len == 3 || len == 7) {
                ++r;
            }
        }

        for (auto &word : left_words) {
            auto len = word.size();

            for (auto &digit : digits) {
                if (digit.original.size() == len) {
                    digit.matches.push_back(word);
                }
            }
        }

        do {

            bool fine = true;
            for (auto &digit : digits) {
                if (!digit.accept(solution)) {
                    fine = false;
                }
            }

            if (fine == true) {
                reverse_solution = solution;
                int i = 0;
                for (auto c : solution) {
                    reverse_solution[c - 'a'] = 'a' + i;
                    ++i;
                }
                break;
            }

        } while (std::next_permutation(solution.begin(), solution.end()));

        int value = 0;

        int i = 0;
        for (auto &word : right_words) {
            for (auto &digit : digits) {
                if (digit.match(reverse_solution, word)) {
                    value += std::pow(10, 3 - i) * digit.value;
                    ++i;
                    break;
                }
            }
        }

        r2 += value;
    }

    log << "Part1: " << r << endl;
    log << "Part2: " << r2 << endl;
}

int main() {
    process("sample2.txt");
    process("sample.txt");
    process("input.txt");
    return 0;
}
