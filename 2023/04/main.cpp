#include "../../common_fast.h"

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

    void parse(const StringView lines) {
        int card_index = 0;

        auto filterEmpty = [](auto range) {
            auto line = getStringView(range);
            return line.size() > 0;
        };

        auto toInt = [](auto range) {
            auto line = getStringView(range);

            int result;

            std::from_chars(line.data(), line.data() + line.size(), result);
            return result;
        };

        auto toCard = [&](auto range) {
            auto line = range;
            Card card;

            card.id = ++card_index;

            auto split = rs::split_string_view(line, ':');
            auto it = std::next(split.begin());

            auto parts = rs::split_string_view(*it, '|');

            auto first = *parts.begin();
            auto second = *std::next(parts.begin());

            std::ranges::copy(rs::split_string_view(first, ' ') | rv::filter(filterEmpty) | rv::transform(toInt),
                              std::back_inserter(card.winningNumbers));
            std::ranges::copy(rs::split_string_view(second, ' ') | rv::filter(filterEmpty) | rv::transform(toInt),
                              std::back_inserter(card.numbers));

            return card;
        };

        auto view = rs::split_string_view(lines, '\n') | rv::transform(toCard);
        std::ranges::copy(view, std::back_inserter(cards));
    }

    void part1() {
        auto result{0};

        for (auto &card : cards) {
            result += card.compute();
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        Vector<int> copies(cards.size(), 0);

        for (auto &card : cards) {
            auto repeats = copies[card.id - 1] + 1;
            auto wins = card.wins();

            for (int w = 0; w < wins; w++) {
                copies[card.id + w + 1 - 1] += repeats;
            }
        }

        auto result = std::accumulate(copies.begin(), copies.end(), 0);

        result += cards.size();

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileContent(filename.c_str());
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

#include <chrono>

int main() {
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
