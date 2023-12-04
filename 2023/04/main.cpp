#include "../../common.h"

struct Card {
    int id;
    Vector<int> winningNumbers;
    Vector<int> numbers;

    int compute() {
        auto result = 0;
        auto score = 1;

        for (auto n : numbers) {

            if (std::find(winningNumbers.begin(), winningNumbers.end(), n) != winningNumbers.end()) {

                if (!result) {
                    result = 1;
                } else {
                    result *= 2;
                }
            }
        }

        return result;
    }

    int wins() {
        auto result = 0;

        for (auto n : numbers) {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), n) != winningNumbers.end()) {
                result++;
            }
        }

        return result;
    }
};

struct Context {

    Vector<Card> cards;

    void parse(const Strings &lines) {
        for (auto &line : lines) {
            auto &card = cards.emplace_back();
            card.id = cards.size();

            auto split = splitString(line, ':');
            auto str = split[1];

            auto parts = splitString(str, '|');

            card.winningNumbers = splitString<int>(parts[0], ' ');
            card.numbers = splitString<int>(parts[1], ' ');
        }
    }

    void part1() {
        auto result{0};

        for (auto &card : cards) {

            result += card.compute();
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        Map<int, int> copies;

        for (auto &card : cards) {
            copies[card.id] += 1;

            auto repeats = copies[card.id];

            auto wins = card.wins();

            for (int w = 0; w < wins; w++) {
                copies[card.id + w + 1] += repeats;
            }
        }

        auto result{0};
        for (auto kv : copies) {

            result += kv.second;
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
        context.part2();
    }
}

int main() {
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
